#include "discovery_subservice.hpp"
#include "../global.hpp"

DiscoverySubservice::DiscoverySubservice(string localHostname, string localIP, string localMAC, string localStatus){
    this->socket = PORTA_DESCOBERTA;
    this->isActive = true;
    this->localHostname = localHostname;
    this->localIP = localIP;
    this->localMAC = localMAC;
    this->localStatus = localStatus;
};

DiscoverySubservice::~DiscoverySubservice(){
   // this->setNotActive();
};

void DiscoverySubservice::setActive(){
    this->isActive = true;
    cout << "DiscoverySubservice>setActive> DiscoverySubservice is active" << endl;
};

void DiscoverySubservice::setNotActive(){
    this->isActive = false;
    cout << "DiscoverySubservice>setNotActive> DiscoverySubservice is not active" << endl;
};

int DiscoverySubservice::serverDiscoverySubservice(participante*& tabelaParticipantes) {

    cout << "DiscoverySubservice>serverDiscoverySubservice> DiscoverySubservice is starting" << endl;

    //create a socket to listen to the discovery port
    SocketAPI serverSocket(PORTA_DESCOBERTA, "server");

    cout << "DiscoverySubservice>serverDiscoverySubservice> DiscoverySubservice created a socketfd:" << serverSocket.getSocketfd() << endl;

    packet_struct packet_received;

    //loop to listen to the socket waiting for SYN packets
    this->setActive();

    cout << "DiscoverySubservice>serverDiscoverySubservice> DiscoverySubservice is listenning" << endl;

    while(this->isActive){
        //passive listening to the socket
        int n = serverSocket.listenSocket(&packet_received);
        if (n < 0) {
            cout << "DiscoverySubservice>serverDiscoverySubservice> error on listenning = " << strerror(errno) << endl;
            return -1;
        }

        //include the new participant in the table
        string newHostname = packet_received.hostname;
        string newIP = packet_received.ip_src;
        string newMAC = packet_received.mac_src;
        string newStatus = packet_received.status;


        if(estaNaTabela(tabelaParticipantes, newMAC) == false){
            novoParticipante(tabelaParticipantes, newHostname, newIP, newMAC, newStatus);
        }
        
        //send an ACK packet to the new participant
        uint seqNum = 0;
        packet_struct returnPacket = createPacket(seqNum,PORTA_DESCOBERTA, PORTA_DESCOBERTA, newIP, this->localIP, this->localHostname, this->localMAC, this->localStatus, ACK);
        n = serverSocket.sendPacket(&returnPacket, newIP, PORTA_DESCOBERTA);

        if (n < 0) {
            cout << "DiscoverySubservice>serverDiscoverySubservice> error on sending ACK" << endl;
            return -1;
        }
    }
    return 0;
};

int DiscoverySubservice::clientDiscoverySubservice() {
    //create socket to broadcast
    SocketAPI clientSocket(PORTA_DESCOBERTA, "client");
    uint seqNum = 0;
    packet_struct synPacket = createPacket(seqNum,PORTA_DESCOBERTA, PORTA_DESCOBERTA, GLOBAL_BROADCAST_ADD, this->localIP, this->localHostname, this->localMAC, this->localStatus, SYN);
    packet_struct ackPacket;
    
    //loop to send SYN packets to the broadcast address until receive an ACK
    this->setActive();
    while(this->isActive){
        //send a SYN packet to the broadcast address
        int n = clientSocket.sendPacket(&synPacket, GLOBAL_BROADCAST_ADD, PORTA_DESCOBERTA);
        if (n < 0) {
            cout << "DiscoverySubservice>clientDiscoverySubservice> error on sending SYN = " << strerror(errno) << endl;
            return -1;
        }

        n = 0;
        auto startTimer = chrono::steady_clock::now();
        const int timeout = 5;

        //listen to the socket for 5 seconds or until receive an ACK
        while ( n <= 0 || chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - startTimer).count() < timeout){
            //passive listening to the socket waiting for ACK packet
            n = clientSocket.listenSocket(&ackPacket);
            if (n < 0) {
                cout << "DiscoverySubservice>clientDiscoverySubservice> error on listenning for ACK = "<< strerror(errno) << endl;
                return -1;
            }
            //behavior when receive an ACK
            if (ackPacket.message == ACK){
                this->setNotActive();
                return n;
            }
        }
    }
    return 0;
};
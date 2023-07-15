#include "discovery_subservice.hpp"

DiscoverySubservice::DiscoverySubservice(participante* tabelaParticipantes, char* localHostname, char* localIP, char* localMAC, char* localStatus){
    this->tabelaParticipantes = tabelaParticipantes;
    this->socket = PORTA_DESCOBERTA;
    this->isActive = false;
    this->localHostname = localHostname;
    this->localIP = localIP;
    this->localMAC = localMAC;
    this->localStatus = localStatus;
};

DiscoverySubservice::~DiscoverySubservice(){
    this->setNotActive();
};

void DiscoverySubservice::setActive(){
    this->isActive = true;
};

void DiscoverySubservice::setNotActive(){
    this->isActive = false;
};

int DiscoverySubservice::serverDiscoverySubservice() {
    //create a socket to listen to the discovery port
    SocketAPI serverSocket(PORTA_DESCOBERTA, "server");
    packet_struct packet_received;

    //loop to listen to the socket waiting for SYN packets
    this->setActive();
    while(this->isActive){
        //passive listening to the socket
        int n = serverSocket.listenSocket(&packet_received);
        if (n < 0) {
            cout << "DiscoverySubservice>serverDiscoverySubservice> error on listenning" << endl;
            return -1;
        }

        //include the new participant in the table
        string newHostname = packet_received.hostname;
        string newIP = packet_received.ip_src;
        string newMAC = packet_received.mac_src;
        string newStatus = packet_received.status;

        if(estaNaTabela(this->tabelaParticipantes, newMAC) == false){
            novoParticipante(this->tabelaParticipantes, newHostname, newIP, newMAC, newStatus);
        }
        
        //send an ACK packet to the new participant
        uint seqNum = 0;
        packet_struct returnPacket = createPacket(seqNum,PORTA_DESCOBERTA, PORTA_DESCOBERTA, (char*)newIP.c_str(), this->localIP, this->localHostname, this->localMAC, this->localStatus, ACK);
        n = serverSocket.sendPacket(returnPacket, (char*)newIP.c_str(), PORTA_DESCOBERTA);

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
    packet_struct synPacket = createPacket(0,PORTA_DESCOBERTA, PORTA_DESCOBERTA, GLOBAL_BROADCAST_ADD, this->localIP, this->localHostname, this->localMAC, this->localStatus, SYN);
    
    //loop to send SYN packets to the broadcast address until receive an ACK
    this->setActive();
    while(this->isActive){
        //send a SYN packet to the broadcast address
        uint seqNum = 0;
        packet_struct packet = createPacket(seqNum, PORTA_DESCOBERTA, PORTA_DESCOBERTA, GLOBAL_BROADCAST_ADD, this->localIP, this->localHostname, this->localMAC, this->localStatus, SYN);
        int n = clientSocket.sendPacket(packet, GLOBAL_BROADCAST_ADD, PORTA_DESCOBERTA);
        if (n < 0) {
            cout << "DiscoverySubservice>clientDiscoverySubservice> error on sending SYN" << endl;
            return -1;
        }

        //passive listening to the socket
        n = clientSocket.listenSocket(&packet_received);
        if (n < 0) {
            cout << "DiscoverySubservice>clientDiscoverySubservice> error on listenning" << endl;
            return -1;
        }

        //include the new participant in the table
        string newHostname = packet_received.hostname;
        string newIP = packet_received.ip_src;
        string newMAC = packet_received.mac_src;
        string newStatus = packet_received.status;

        if(estaNaTabela(this->tabelaParticipantes, newMAC) == false){
            novoParticipante(this->tabelaParticipantes, newHostname, newIP, newMAC, newStatus);
        }
    }
    return 0;
};
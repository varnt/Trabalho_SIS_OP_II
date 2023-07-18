#include "discovery_subservice.hpp"
#include "../global.hpp"

DiscoverySubservice::DiscoverySubservice(string localHostname, string localIP, string localMAC, string localStatus)
{
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

void DiscoverySubservice::setActive()
{
    this->isActive = true;
    cout << "DiscoverySubservice>setActive> DiscoverySubservice is active" << endl;
};

void DiscoverySubservice::setNotActive()
{
    this->isActive = false;
    cout << "DiscoverySubservice>setNotActive> DiscoverySubservice is not active" << endl;
};

int DiscoverySubservice::serverDiscoverySubservice(participante *&tabelaParticipantes)
{

    cout << "DiscoverySubservice>serverDiscoverySubservice> DiscoverySubservice is starting" << endl;

    // create a socket to listen to the discovery port
    SocketAPI serverSocket(PORTA_DESCOBERTA, "server");

    cout << "DiscoverySubservice>serverDiscoverySubservice> DiscoverySubservice created a socketfd:" << serverSocket.getSocketfd() << endl;

    packet_struct packet_received;

    // loop to listen to the socket waiting for SYN packets
    this->setActive();

    cout << "DiscoverySubservice>serverDiscoverySubservice> DiscoverySubservice is listenning" << endl;

    while (this->isActive)
    {
        // passive listening to the socket
        int n = 0; // num of bytes received
        while (n <= 0)
        {
            n = serverSocket.listenSocket(&packet_received);
            if (n < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    n = 0;
                }
                else
                {
                    cout << "DiscoverySubservice>serverDiscoverySubservice> error on listening socket" << endl;
                    return -1;
                }
            }
            else if (n > 0)
            {
                string newHostname = packet_received.hostname;
                string newIP = packet_received.ip_src;
                string newMAC = packet_received.mac_src;
                string newStatus = packet_received.status;
                uint16_t srcPort = packet_received.src_port;

                cout << "srcPort = " << srcPort << endl;

                if (estaNaTabela(tabelaParticipantes, newMAC) == false)
                {
                    // include the new participant in the table
                    novoParticipante(tabelaParticipantes, newHostname, newIP, newMAC, newStatus);
                    cout << "DiscoverySubservice>serverDiscoverySubservice> packet ip_src = " << packet_received.ip_src << endl;
                }

                // send an ACK packet to the new participant
                uint seqNum = 0;
                packet_struct returnPacket = createPacket(seqNum, srcPort, PORTA_DESCOBERTA, newIP, this->localIP, this->localHostname, this->localMAC, this->localStatus, ACK);
                n = serverSocket.sendPacket(&returnPacket, newIP, srcPort);

                if (n < 0)
                {
                    cout << "DiscoverySubservice>serverDiscoverySubservice> error on sending ACK" << endl;
                }
            }
        }
    }

    printList(tabelaParticipantes);
    return 0;
};

int DiscoverySubservice::clientDiscoverySubservice()
{
    cout << "DiscoverySubservice>clientDiscoverySubservice> DiscoverySubservice is starting" << endl;
    // create socket to broadcast
    SocketAPI clientSocket(PORTA_DESCOBERTA, "client");
    cout << "DiscoverySubservice>clientDiscoverySubservice> DiscoverySubservice created a socketfd:" << clientSocket.getSocketfd() << endl;
    uint seqNum = 0;
    packet_struct synPacket = createPacket(seqNum, PORTA_DESCOBERTA, PORTA_DESCOBERTA, GLOBAL_BROADCAST_ADD, this->localIP, this->localHostname, this->localMAC, this->localStatus, SYN);
    cout << "DiscoverySubservice>clientDiscoverySubservice> DiscoverySubservice created a SYN packet" << endl;
    packet_struct ackPacket;

    // loop to send SYN packets to the broadcast address until receive an ACK
    this->setActive();
    while (this->isActive)
    {
        // send a SYN packet to the broadcast address
        cout << "DiscoverySubservice>clientDiscoverySubservice> DiscoverySubservice is sending SYN" << endl;
        int n = clientSocket.sendPacket(&synPacket, GLOBAL_BROADCAST_ADD, PORTA_DESCOBERTA);
        if (n < 0)
        {
            cout << "DiscoverySubservice>clientDiscoverySubservice> error on sending SYN = " << strerror(errno) << endl;
            return -1;
        }

        n = 0;
        // listen to the socket for 5 seconds or until receive an ACK
        while (n <= 0)
        {
            // passive listening to the socket waiting for ACK packet

            cout << "DiscoverySubservice>clientDiscoverySubservice> DiscoverySubservice is listenning" << endl;
            n = clientSocket.listenSocket(&ackPacket);
            if (n < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    n = 1;
                }
                else
                {
                    cerr << "DiscoverySubservice>clientDiscoverySubservice> error on listenning for ACK = " << strerror(errno) << endl;
                    return -1;
                }
            }
            // behavior when receive an ACK
            if (ackPacket.message == ACK)
            {
                this->setNotActive();
            }
        }
    }
    return 0;
};
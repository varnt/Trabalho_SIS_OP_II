#include "discovery_subservice.hpp"

DiscoverySubservice::DiscoverySubservice(participante* tabelaParticipantes){
    this->tabelaParticipantes = tabelaParticipantes;
    this->socket = PORTA_DESCOBERTA;
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
    SocketAPI serverSocket(PORTA_DESCOBERTA, "server");
    packet_struct packet_received;
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
        
    }

    serverSocket.~SocketAPI();
    return 0;


};
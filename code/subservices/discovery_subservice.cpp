#include "discovery_subservice.hpp"
#include "../global.hpp"

DiscoverySubservice::DiscoverySubservice(bool *tabelaParticipantesUpdate,
                                         string localHostname, string localIP,
                                         string localMAC, string localStatus) {
  this->tabelaEstaAtualizada = tabelaParticipantesUpdate;
  this->socket = PORTA_DESCOBERTA;
  this->isActive = true;
  this->localHostname = localHostname;
  this->localIP = localIP;
  this->localMAC = localMAC;
  this->localStatus = localStatus;
};

DiscoverySubservice::~DiscoverySubservice(){};

void DiscoverySubservice::setActive() { this->isActive = true; };

void DiscoverySubservice::setNotActive() { this->isActive = false; };

int DiscoverySubservice::serverDiscoverySubservice(
    participante *&tabelaParticipantes) {

  // create a socket to listen to the discovery port
  SocketAPI serverSocket(PORTA_DESCOBERTA, "server");

  packet_struct packet_received;

  // loop to listen to the socket waiting for SYN packets
  this->setActive();

  while (this->isActive) {
    // passive listening to the socket
    int n = 0; // num of bytes received
    while (n <= 0) {
      n = serverSocket.listenSocket(&packet_received);
      if (n < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          n = 0;
        } else {
          cerr << "DiscoverySubservice>serverDiscoverySubservice> error on "
                  "listening socket"
               << endl;
          return -1;
        }
      } else if (n > 0) {
        string newHostname = packet_received.hostname;
        string newIP = packet_received.ip_src;
        string newMAC = packet_received.mac_src;
        string newStatus = packet_received.status;
        uint16_t srcPort = packet_received.src_port;

        if (estaNaTabela(tabelaParticipantes, newMAC) == false) {
          // include the new participant in the table
          mtx.lock();
          table_mtx.lock();
          novoParticipante(tabelaParticipantes, newHostname, newIP, newMAC,
                           newStatus);
          mtx.unlock();
          *tabelaEstaAtualizada = false;
          table_mtx.unlock();
        } else if (packet_received.message == EXIT_MSG) {
          mtx.lock();
          table_mtx.lock();
          excluirParticipante(tabelaParticipantes, newMAC);
          mtx.unlock();
          *tabelaEstaAtualizada = false;
          table_mtx.unlock();
        }

        // send an ACK packet to the new participant
        uint seqNum = 0;
        packet_struct returnPacket = createPacket(
            seqNum, srcPort, PORTA_DESCOBERTA, newIP, this->localIP,
            this->localHostname, this->localMAC, this->localStatus, ACK);
        n = serverSocket.sendPacket(&returnPacket, newIP, srcPort);

        if (n < 0) {
          cerr << "DiscoverySubservice>serverDiscoverySubservice> error on "
                  "sending ACK"
               << endl;
        }
      }
    }
  }

  printList(tabelaParticipantes);
  return 0;
};

int DiscoverySubservice::clientDiscoverySubservice() {
  // create socket to broadcast
  SocketAPI clientSocket(PORTA_DESCOBERTA_CLIENTE, "client");
  uint seqNum = 0;
  packet_struct synPacket =
      createPacket(seqNum, PORTA_DESCOBERTA, PORTA_DESCOBERTA_CLIENTE,
                   GLOBAL_BROADCAST_ADD, this->localIP, this->localHostname,
                   this->localMAC, this->localStatus, SYN);
  packet_struct ackPacket;

  // loop to send SYN packets to the broadcast address until receive an ACK
  this->setActive();
  while (this->isActive) {
    // send a SYN packet to the broadcast address
    int n = clientSocket.sendPacket(&synPacket, GLOBAL_BROADCAST_ADD,
                                    PORTA_DESCOBERTA);
    if (n < 0) {
      cerr << "DiscoverySubservice>clientDiscoverySubservice> error on sending "
              "SYN = "
           << strerror(errno) << endl;
      return -1;
    }

    n = 0;
    // listen to the socket for 5 seconds or until receive an ACK
    while (n <= 0) {
      // passive listening to the socket waiting for ACK packet

      n = clientSocket.listenSocket(&ackPacket);
      if (n < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          n = 1;
        } else {
          cerr << "DiscoverySubservice>clientDiscoverySubservice> error on "
                  "listenning for ACK = "
               << strerror(errno) << endl;
          return -1;
        }
      }
      // behavior when receive an ACK
      if (ackPacket.message == ACK) {
        this->setNotActive();
      }
    }
  }
  return 0;
};

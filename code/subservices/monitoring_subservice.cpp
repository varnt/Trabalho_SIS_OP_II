#include "monitoring_subservice.hpp"
#include "../common/packet.hpp"
#include "../common/socketAPI.hpp"

MonitoringSubservice::MonitoringSubservice(
    participante *&tabelaParticipantes, bool *tabelaParticipantesUpdate,
    string localHostname, string localIpAddress, string localMacAddress,
    string localStatus, string sessionMode) {
  this->tabelaParticipantes = tabelaParticipantes;
  this->tabelaEstaAtualizada = tabelaParticipantesUpdate;
  this->localHostname = localHostname;
  this->localIpAddress = localIpAddress;
  this->localMacAddress = localMacAddress;
  this->localStatus = localStatus;
  this->sessionMode = sessionMode;
};
MonitoringSubservice::~MonitoringSubservice(){};

bool MonitoringSubservice::isActive() { return this->currentState; };
void MonitoringSubservice::setActive() { this->currentState = true; };
void MonitoringSubservice::setNotActive() { this->currentState = false; };

int MonitoringSubservice::serverMonitoringSubservice() {
  this->setActive();
  while (this->isActive()) {
    participante *currparticipante = this->tabelaParticipantes;
    SocketAPI socket(PORTA_GERENCIA, "server");
    while (currparticipante != nullptr) {
      int i;
      uint seqNum = 0;
      packet_struct packet_sent = createPacket(
          seqNum, PORTA_GERENCIA_CLIENTE, PORTA_GERENCIA,
          currparticipante->ip_address, this->localIpAddress,
          this->localHostname, this->localMacAddress, this->localStatus, SYN);
      for (i = 0; i < 3; i++) {

        int m = socket.sendPacket(&packet_sent, currparticipante->ip_address,
                                  PORTA_GERENCIA_CLIENTE); // end, porta
        if (m < 0) {
          cerr << "ManagementSubservice>serverManagementSubservice> Error "
                  "sending packet"
               << endl;
          return -1;
        }

        int n = 0;
        int j = 0;
        packet_struct ackPacket;
        while (n <= 0 && j < 3) {

          // passive listening to the socket waiting for ACK packet
          n = socket.listenSocket(&ackPacket);
          if (n < 0) {
            ackPacket.message = -2;

            table_mtx.lock();
            mtx.lock();
            currparticipante->status = "ASLEEP";
            mtx.unlock();
            table_mtx.unlock();

            if (errno == EAGAIN || errno == EWOULDBLOCK) {
              n = 1;
            } else {
              cerr << "ManagementSubservice>serverManagementSubservice> Error "
                      "listening to socket"
                   << endl;
              return -1;
            }
          } else {
            if (ackPacket.message == ACK) {
              // behavior when receive an ACK
              table_mtx.lock();
              mtx.lock();
              currparticipante->status = "awaken";
              table_mtx.unlock();
              mtx.unlock();
              break;
            } else {
              cerr << "ManagementSubservice>serverManagementSubservice> No "
                      "packet received ------------ must be sleeping"
                   << endl;
            table_mtx.lock();
            mtx.lock();
            currparticipante->status = "ASLEEP";
            mtx.unlock();
            table_mtx.unlock();
            }
          }
          j++;
        }
      }

      currparticipante = currparticipante->next;

      if (currparticipante == nullptr) {
        break;
      }
    }
    this_thread::sleep_for(chrono::seconds(MONITORING_FREQUENCY_SEC));
    table_mtx.lock();
    *tabelaEstaAtualizada = false;
    table_mtx.unlock();
  }
  return 0;
};

int MonitoringSubservice::clientMonitoringSubservice() {
  SocketAPI socket(PORTA_GERENCIA_CLIENTE, "client");
  packet_struct packet_received;

  int n = 0;
  this->setActive();
  while (this->isActive()) {
    n = socket.listenSocket(&packet_received);
    if (n < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        n = 1;
      } else {
        // cerr << "MonitoringSubservice>clientMonitoringSubservice> Error "
        //         "listening to socket"
        //      << endl;
        // return -1;
      }
    } else if (n > 0) {
      if (packet_received.message == SYN) {
        int seqNum = 0;
        packet_struct packet_sent = createPacket(
            seqNum, PORTA_GERENCIA, PORTA_GERENCIA_CLIENTE,
            packet_received.ip_src, this->localIpAddress, this->localHostname,
            this->localMacAddress, this->localStatus, ACK);
        n = socket.sendPacket(&packet_sent, packet_sent.ip_dest,
                              PORTA_GERENCIA);
        if (n < 0) {
          // cerr << "MonitoringSubservice>clientMonitoringSubservice> Error "
          //         "sending packet"
          //      << endl;
          // return -1;
        }
      }
    }
  }

  return 0;
};

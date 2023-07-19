#include "monitoring_subservice.hpp"
#include "../common/socketAPI.hpp"
#include "../common/packet.hpp"

MonitoringSubservice::MonitoringSubservice(participante *&tabelaParticipantes, bool *tabelaParticipantesUpdate, string localHostname, string localIpAddress, string localMacAddress, string localStatus, string sessionMode)
{
    this->tabelaParticipantes = tabelaParticipantes;
    this->tabelaEstaAtualizada = tabelaParticipantesUpdate;
    this->localHostname = localHostname;
    this->localIpAddress = localIpAddress;
    this->localMacAddress = localMacAddress;
    this->localStatus = localStatus;
    this->sessionMode = sessionMode;
};
MonitoringSubservice::~MonitoringSubservice(){};

bool MonitoringSubservice::isActive()
{
    return this->currentState;
};
void MonitoringSubservice::setActive()
{
    this->currentState = true;
};
void MonitoringSubservice::setNotActive()
{
    this->currentState = false;
};

int MonitoringSubservice::serverMonitoringSubservice()
{
    this->setActive();
    while (this->isActive())
    {
        participante *currparticipante = this->tabelaParticipantes;
        SocketAPI socket(PORTA_GERENCIA, "server");
        cout << "ManagementSubservice>serverManagementSubservice> ManagementSubservice starting" << endl;
        while (currparticipante != nullptr)
        {
            int i;
            uint seqNum = 0;
            packet_struct packet_sent = createPacket(seqNum, PORTA_GERENCIA_CLIENTE, PORTA_GERENCIA, currparticipante->ip_address, this->localIpAddress, this->localHostname, this->localMacAddress, this->localStatus, SYN);
            for (i = 0; i < 3; i++)
            {
                int seqNum = 0;
                int n = socket.sendPacket(&packet_sent, currparticipante->ip_address, PORTA_GERENCIA_CLIENTE); // end, porta
                if (n < 0)
                {
                    cout << "ManagementSubservice>serverManagementSubservice> Error sending packet" << endl;
                    return -1;
                }

                n = 0;
                int j = 0;
                packet_struct ackPacket;
                while (n <= 0 && j < 3)
                {
                    // passive listening to the socket waiting for ACK packet
                    n = socket.listenSocket(&ackPacket);
                    if (n < 0)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            n = 1;
                        }
                        else
                        {
                            cout << "ManagementSubservice>serverManagementSubservice> Error listening to socket" << endl;
                            return -1;
                        }
                    }
                    // behavior when receive an ACK
                    if (ackPacket.message == ACK)
                    {
                        currparticipante->status = "awaken";
                    }
                    else
                    {
                        currparticipante->status = "sleep";
                    }
                    j++;
                }
                currparticipante = currparticipante->next;
                if (currparticipante == nullptr) {
        break;
    }
            }
            *tabelaEstaAtualizada = false;
        }
        this_thread::sleep_for(chrono::seconds(5));
    }
    return 0;
};

int MonitoringSubservice::clientMonitoringSubservice()
{
    SocketAPI socket(PORTA_GERENCIA_CLIENTE, "client");
    packet_struct packet_received;

    int n = 0;
    this->setActive();
    while (this->isActive())
    {
        n = socket.listenSocket(&packet_received);
        if (n < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                n = 1;
            }
            else
            {
                cout << "MonitoringSubservice>clientMonitoringSubservice> Error listening to socket" << endl;
                return -1;
            }
        }
        else if (n > 0)
        {
            if (packet_received.message == SYN)
            {
                int seqNum = 0;
                packet_struct packet_sent = createPacket(seqNum, PORTA_GERENCIA, PORTA_GERENCIA_CLIENTE, packet_received.ip_dest, this->localIpAddress, this->localHostname, this->localMacAddress, this->localStatus, ACK);
                n = socket.sendPacket(&packet_sent, packet_received.ip_dest, PORTA_GERENCIA);
                if (n < 0)
                {
                    cout << "MonitoringSubservice>clientMonitoringSubservice> Error sending packet" << endl;
                    return -1;
                }
            }
        }
    }

    return 0;
};

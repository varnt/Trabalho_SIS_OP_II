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

                int m = socket.sendPacket(&packet_sent, currparticipante->ip_address, PORTA_GERENCIA_CLIENTE); // end, porta
                if (m < 0)
                {
                    cout << "ManagementSubservice>serverManagementSubservice> Error sending packet" << endl;
                    return -1;
                }
                cout << "sending syn packet to " << currparticipante->ip_address << endl;

                int n = 0;
                int j = 0;
                packet_struct ackPacket;
                while (n <= 0 && j < 3)
                {

                    // passive listening to the socket waiting for ACK packet
                    n = socket.listenSocket(&ackPacket);
                    cout << "ManagementSubservice>serverManagementSubservice> n = " << n << endl;
                    if (n < 0)
                    {
                        ackPacket.message = -2;
                            currparticipante->status = "sleep";

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
                    else 
                    {
                        // cout << "received packet from " << ackPacket.ip_src << endl;
                        // cout << "message = " << ackPacket.message << endl;
                        if (ackPacket.message == ACK)
                        {
                            // behavior when receive an ACK
                            currparticipante->status = "awaken";
                            break;
                        }
                        else
                        {
                        cout << "ManagementSubservice>serverManagementSubservice> No packet received ------------ must be sleeping" << endl;
//  
                            currparticipante->status = "sleep";
                        }
                    }
                    // else
                    // {
                    //     currparticipante->status = "sleep";
                    //     cout << "ManagementSubservice>serverManagementSubservice> No packet received ------------ must be sleeping" << endl;
                    // }
                    j++;
                }
            }

            currparticipante = currparticipante->next;
            if (currparticipante == nullptr)
            {
                break;
            }
        }
        this_thread::sleep_for(chrono::seconds(5));
        *tabelaEstaAtualizada = false;
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
        cout << "listenning on socket = " << PORTA_GERENCIA_CLIENTE << endl;
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
            cout << "received packet from " << packet_received.ip_src << endl;
            cout << "message = " << packet_received.message << endl;
            if (packet_received.message == SYN)
            {
                int seqNum = 0;
                packet_struct packet_sent = createPacket(seqNum, PORTA_GERENCIA, PORTA_GERENCIA_CLIENTE, packet_received.ip_src, this->localIpAddress, this->localHostname, this->localMacAddress, this->localStatus, ACK);
                cout << "sending ACK to " << packet_sent.ip_dest << endl;
                cout << "sending message = " << packet_sent.message << endl;
                n = socket.sendPacket(&packet_sent, packet_sent.ip_dest, PORTA_GERENCIA);
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

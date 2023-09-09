#include "replica_subservice.hpp"
#include "../common/packet.hpp"
#include "../common/socketAPI.hpp"

ReplicaSubservice::MonitoringSubservice(
    participante *&tabelaParticipantes, bool *tabelaParticipantesUpdate,
    string localHostname, string localIpAddress, string localMacAddress,
    string localStatus, string sessionMode)
{
    this->tabelaParticipantes = tabelaParticipantes;
    this->tabelaEstaAtualizada = tabelaParticipantesUpdate;
    this->localHostname = localHostname;
    this->localIpAddress = localIpAddress;
    this->localMacAddress = localMacAddress;
    this->localStatus = localStatus;
    this->sessionMode = sessionMode;
    this->latestReplica = 0;
};
ReplicaSubservice::~ReplicaSubservice(){};

bool ReplicaSubservice::isActive() { return this->currentState; };
void ReplicaSubservice::setActive() { this->currentState = true; };
void ReplicaSubservice::setNotActive() { this->currentState = false; };

int ReplicaSubservice::serverReplicaSubservice()
{
    this->setActive();
    while (this->isActive())
    {
        uint seqNum = 0;
        uint64_t replica_timestamp = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        participante *currparticipante = this->tabelaParticipantes;
        SocketAPI socket(PORTA_REPLICA, "server");
        while (currparticipante != nullptr)
        {
            replica_struct replica_packet = createReplicaPkt(seqNum, PORTA_REPLICA_CLIENTE, PORTA_REPLICA, this->localIpAddress, currparticipante->id_address, currparticipante->mac_address, currparticipante->status, replica_timestamp, SYN);
            int m = socket.sendPacket(&replica_packet, GLOBAL_BROADCAST_ADD, PORTA_REPLICA_CLIENTE);
            if (m < 0)
            {
                cerr << "ReplicaSubservice>serverReplicaSubservice> Error sending packet" << endl;
                return -1;
            }

            replica_struct ackReplicaPacket;
            int n = 0;
            n = socket.listenSocket(&ackReplicaPacket);
            if (n < 0) {
                //TRATAR CASO DE NÂO TER REPLICA MANAGERS
            }
            else if (n > 0)
            {
                currparticipante = currparticipante->next;
            }
        }
    }
    return 0;
};

int ReplicaSubservice::clientReplicaSubservice()



{
    SocketAPI socket(PORTA_REPLICA_CLIENTE, "client");
    replica_struct replica_packet_received;

    int n = 0;
    this->setActive();
    while (this->isActive())
    {
        n = socket.listenSocket(&replica_packet_received);
        if (n < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                n = 1;
            }
        }
        else if (n > 0)
        {
            if (replica_packet_received.message == SYN)
            {
                if (estaNaTabela(this->tabelaParticipantes, packet_received.part_mac))
                {
                    setStatusTabela(this->tabelaParticipantes, packet_received.part_mac, packet_received.part_status);
                }
                else
                {
                    novoParticipanteID(this->tabelaParticipantes, replica_packet_received.id, replica_packet_received.hostname, replica_packet_received.ip_src, replica_packet_received.mac_address, replica_packet_received.status);
                }


                int seqNum = 0;
                replica_packet_received.message = ACK;
                n = socket.sendPacket(&replica_packet_received, replica_packet_received.ip_src,
                                      PORTA_REPLICA);
            }
        }
    }

    return 0;
};

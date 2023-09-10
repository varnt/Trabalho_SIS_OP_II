#include "replica_subservice.hpp"
#include "../common/packet.hpp"
#include "../common/socketAPI.hpp"
#include <chrono>

ReplicaSubservice::ReplicaSubservice(
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
        uint64_t replica_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        participante *currparticipante = this->tabelaParticipantes;
        SocketAPI socket(PORTA_REPLICA, "server");
        while (currparticipante != nullptr)
        {
            replica_struct replica_packet = createReplicaPkt(seqNum, PORTA_REPLICA_CLIENTE, PORTA_REPLICA, this->localIpAddress, currparticipante->id, currparticipante->hostname, currparticipante->ip_address, currparticipante->mac_address, currparticipante->status, replica_timestamp, SYN);
            int m = socket.sendReplicaPacket(&replica_packet, GLOBAL_BROADCAST_ADD, PORTA_REPLICA_CLIENTE);
            if (m < 0)
            {
                cerr << "ReplicaSubservice>serverReplicaSubservice> Error sending packet" << endl;
                return -1;
            }

            replica_struct ackReplicaPacket;
            int n = 0;
            n = socket.listenReplicaSocket(&ackReplicaPacket);
            if (n < 0) {
                //TRATAR CASO DE NÂO TER REPLICA MANAGERS
            }
            else if (n > 0)
            {
                cout << "REPLICA RECEBIDA = " << endl;
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
        n = socket.listenReplicaSocket(&replica_packet_received);
        if (n < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                n = 1;
            }
        }
        else if (n > 0)
        {
            cout << "REPLICA RECEBIDA IP =  " << replica_packet_received.ip_src << endl;
            cout << "REPLICA RECEBIDA MENSAGEM =  " << replica_packet_received.message << endl;
            if (replica_packet_received.message == SYN && replica_packet_received.ip_src == MANAGER_IP_ADDRESS)
            {
                cout << "REPLICA IP PART =  " << replica_packet_received.part_ip << endl;
                if (estaNaTabela(this->tabelaParticipantes, replica_packet_received.part_mac) == true)
                {
                    cout << "ESTA NA TABELA" << endl;
                    setStatusTabela(this->tabelaParticipantes, replica_packet_received.part_mac, replica_packet_received.part_status, replica_packet_received.part_id);
                }
                else
                {
                    cout << "NAO ESTA NA TABELA" << endl;
                    novoParticipanteID(this->tabelaParticipantes, replica_packet_received.part_id, replica_packet_received.part_hostname, replica_packet_received.ip_src, replica_packet_received.part_mac, replica_packet_received.part_status);
                }


                int seqNum = 0;
                n = socket.sendReplicaPacket(&replica_packet_received, replica_packet_received.ip_src, PORTA_REPLICA);
            }
        }
    }

    return 0;
};

#include "replica_subservice.hpp"
#include "../common/packet.hpp"
#include "../common/socketAPI.hpp"
#include <chrono>

ReplicaSubservice::ReplicaSubservice(
    participante *&tabelaParticipantes, bool *tabelaParticipantesUpdate,
    string localHostname, string localIpAddress, string localMacAddress,
    string localStatus)
{
    this->tabelaParticipantes = tabelaParticipantes;
    this->tabelaEstaAtualizada = tabelaParticipantesUpdate;
    this->localHostname = localHostname;
    this->localIpAddress = localIpAddress;
    this->localMacAddress = localMacAddress;
    this->localStatus = localStatus;
    this->latestReplica = 0;
};
ReplicaSubservice::~ReplicaSubservice(){};

bool ReplicaSubservice::isActive() { return this->currentState; };
void ReplicaSubservice::setActive() { this->currentState = true; };
void ReplicaSubservice::setNotActive() { this->currentState = false; };

int ReplicaSubservice::serverReplicaSubservice()
{
    SocketAPI socket(PORTA_REPLICA, "server");
    this->setActive();
    while (this->isActive() && sessionMode != "client")
    {
        uint seqNum = 0;
        uint64_t replica_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        participante *currparticipante = this->tabelaParticipantes;
        int attempts = 0;
        while (currparticipante != nullptr)
        {
            replica_struct replica_packet = createReplicaPkt(seqNum, PORTA_REPLICA_CLIENTE, PORTA_REPLICA, this->localIpAddress, currparticipante->id, currparticipante->hostname, currparticipante->ip_address, currparticipante->mac_address, currparticipante->status, replica_timestamp, SYN);
            int m = socket.sendReplicaPacket(&replica_packet, GLOBAL_BROADCAST_ADD, PORTA_REPLICA_CLIENTE);
            if (m < 0)
            {
                cerr << "ReplicaSubservice>serverReplicaSubservice> Error sending packet" << endl;
                replica_status = "off";
                return -1;
            }

            replica_struct ackReplicaPacket;
            int n = 0;
            n = socket.listenReplicaSocket(&ackReplicaPacket);
            if (n <= 0 && attempts >= 10)
            {
                // REALIZA CHECAGEM DE LIVENESS DO MANAGER
                cout << "não recebeu contato dos participantes" << std::endl;
                sessionMode = "client";
                replica_status = "off";
                return -1;
            }
            else if (n < 0 && currparticipante->status == "awaken")
            {
                attempts++;
            }
            else if (n > 0 && currparticipante->ip_address != this->localIpAddress)
            {
                attempts = 0;
                // cout << "received replica packet from ip = " << ackReplicaPacket.part_ip << endl;
            }
            currparticipante = currparticipante->next;
        }
    }
    replica_status = "off";
    return 0;
};

int ReplicaSubservice::clientReplicaSubservice()
{
    SocketAPI socket(PORTA_REPLICA_CLIENTE, "client");
    replica_struct replica_packet_received;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    int n = 0;
    this->setActive();
    while (this->isActive() && sessionMode != "manager")
    {

        n = socket.listenReplicaSocket(&replica_packet_received);
        if (n < 0)
        {
            end = std::chrono::steady_clock::now();
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                n = -1;         
                //cout << "time since last replica received = " << chrono::duration_cast<std::chrono::seconds> (end - begin).count() << endl;
                if (chrono::duration_cast<std::chrono::seconds> (end - begin).count() > 5)
                {
                    cout << "SEM CONTATO DO MANAGER" << endl;
                    isElectionPeriod = true;
                    begin = std::chrono::steady_clock::now();
                }
            }
        }
        else if (n > 0)
        {
            begin = std::chrono::steady_clock::now();
            if (replica_packet_received.message == SYN && replica_packet_received.ip_src == MANAGER_IP_ADDRESS)
            {
                begin = std::chrono::steady_clock::now();
                //cout << "received replica packet from ip = " << replica_packet_received.ip_src << endl;
                if (estaNaTabela(this->tabelaParticipantes, replica_packet_received.part_mac) == true)
                {

                    setStatusTabela(this->tabelaParticipantes, replica_packet_received.part_mac, replica_packet_received.part_status, replica_packet_received.part_id);
                }
                else
                {

                    novoParticipanteID(this->tabelaParticipantes, replica_packet_received.part_id, replica_packet_received.part_hostname, replica_packet_received.ip_src, replica_packet_received.part_mac, replica_packet_received.part_status);
                }
                if (replica_packet_received.part_mac == this->localMacAddress)
                {
                    self_id = replica_packet_received.part_id;
                }
                for (int x = 0; x < 3; x++)
                {
                    int m = socket.sendReplicaPacket(&replica_packet_received, replica_packet_received.ip_src, PORTA_REPLICA);
                    if (m < 0)
                    {
                        cerr << "ReplicaSubservice>eleicaoBully> Error sending packet" << endl;
                        return -1;
                    }
                }
            }
        }
    }

    replica_status = "off";
    return 0;
};

int ReplicaSubservice::activeListening()
{
    isElectionPeriod = false;
    // Thread para recebendo possíveis contatos de eleição
    // Se é de ID maior, não faz nada

    // DECLARAÇÂO DA THREAD  BULLY
    thread ele_thr(&ReplicaSubservice::eleicaoBully, this);
    SocketAPI socket(PORTA_ELEICAO_CLIENTE, "eleicao");

    while (sessionMode != "manager")
    {
        // passive listening to the socket
        packet_struct packet_received;
        int n = 0; // num of bytes received
        while (n <= 0)
        {
            n = socket.listenSocket(&packet_received);
            if (n < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    n = 0;
                }
                else
                {
                    cerr << "DiscoverySubservice>serverDiscoverySubservice> error on "
                            "listening socket"
                         << endl;
                    return -1;
                }
            }
            else if (n > 0)
            {
                if (packet_received.src_port == PORTA_NOVO_LIDER)
                {
                    packet_struct packet = createPacket(self_id, PORTA_ELEICAO, PORTA_ELEICAO_CLIENTE,
                                                        packet_received.ip_src, this->localIpAddress, this->localHostname,
                                                        this->localMacAddress, this->localStatus, ACK);
                    MANAGER_IP_ADDRESS = packet_received.ip_src;
                    for (int x = 0; x < 3; x++)
                    {
                        int m = socket.sendPacket(&packet, MANAGER_IP_ADDRESS, PORTA_NOVO_LIDER);
                        if (m < 0)
                        {
                            cerr << "ReplicaSubservice>eleicaoBully> Error sending packet" << endl;
                            return -1;
                        }
                    }
                }

                if (packet_received.sequence_number > self_id)
                {
                    isElectionPeriod = true;

                    packet_struct packet = createPacket(self_id, PORTA_ELEICAO, PORTA_NOVO_LIDER,
                                                        packet_received.ip_src, this->localIpAddress, this->localHostname,
                                                        this->localMacAddress, this->localStatus, ACK_ELECTION);
                    for (int x = 0; x < 3; x++)
                    {
                        int m = socket.sendPacket(&packet, packet.ip_dest, PORTA_ELEICAO);
                        if (m < 0)
                        {
                            cerr << "ReplicaSubservice>eleicaoBully> Error sending packet" << endl;
                            return -1;
                        }
                    }
                }
            }
        }
    }

    // JOIN THREAD BULLY
    ele_thr.join();

    return 0;
}

int ReplicaSubservice::eleicaoBully()
{
    bool got_elected = false;
    SocketAPI socket(PORTA_ELEICAO, "eleicao");
    while (sessionMode != "manager")
    {
        if (isElectionPeriod == true)
        {
            packet_struct packet_received;
            packet_struct packet = createPacket(self_id, PORTA_ELEICAO_CLIENTE, PORTA_ELEICAO,
                                                GLOBAL_BROADCAST_ADD, this->localIpAddress, this->localHostname,
                                                this->localMacAddress, this->localStatus, SYN_ELECTION);
            // Se é de ID menor, envia um ACK e contata todos os IDs maiores
            // Se ele nota que não tem manager, ou se foi informado que não tem manager, ele inicia uma eleição:
            int j = 0;
            bool got_answered = false;
            cout << "sending election packet" << endl;
            while (j <= 5 && got_answered == false)
            {
                j++;
                int m = socket.sendPacket(&packet, GLOBAL_BROADCAST_ADD, PORTA_ELEICAO_CLIENTE);
                if (m < 0)
                {
                    cerr << "ReplicaSubservice>eleicaoBully> Error sending packet" << endl;
                    return -1;
                }
                cout << "listening for answer" << endl;
                int n = socket.listenSocket(&packet_received);
                if (n > 0)
                {
                    got_answered = true;
                    cout << "got answer" << endl;
                    isElectionPeriod = false;
                }
            }
            if (got_answered == false)
            {
                cout << "got elected" << endl;
                got_elected = true;
                MANAGER_IP_ADDRESS = this->localIpAddress;
                isElectionPeriod = false;
                sessionMode = "manager";
                this->declareNewLeader();
                // TRATAR PARA ENTRAR COMO MANAGER AGORA
                return 0;
            }
        }
    }
    cout << "Closing eleicaoBully" << endl;
    return 0;
}

int ReplicaSubservice::declareNewLeader()
{

    cout << "declaring myself as the new leader" << endl;
    participante *currparticipante = this->tabelaParticipantes;
    SocketAPI socket(PORTA_NOVO_LIDER, "eleicao");
    uint seqNum = 0;
    while (currparticipante != nullptr)
    {
        packet_struct packet = createPacket(seqNum, PORTA_ELEICAO_CLIENTE, PORTA_NOVO_LIDER,
                                            currparticipante->ip_address, this->localIpAddress, this->localHostname,
                                            this->localMacAddress, this->localStatus, NEW_MANAGER);
        int j = 0;
        int n = 0;
        packet_struct packet_received;
        while (n <= 0 && j < 5)
        {
            if (currparticipante->mac_address != this->localMacAddress)
            {
                int m = socket.sendPacket(&packet, currparticipante->ip_address, PORTA_NOVO_LIDER);
                if (m < 0)
                {
                    cerr << "ReplicaSubservice>declareNewLeader> Error sending packet" << endl;
                    //return -1;
                }
                n = socket.listenSocket(&packet_received);
                if (n <= 0)
                {
                    j++;
                }
            }
        }
        currparticipante = currparticipante->next;
    }
    cout << "New leader declaration is done";
    return 0;
    
}
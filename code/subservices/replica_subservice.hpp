#ifndef REPLICA_SUBSERVICE_HPP
#define REPLICA_SUBSERVICE_HPP
#include "../global.hpp"

class ReplicaSubservice
{
public:
    ReplicaSubservice(participante *&tabelaParticipantes, bool *tabelaParticipantesUpdate, string localHostName, string localIpAddress, string localMacAddress, string localStatus, string sessionMode);
    ~ReplicaSubservice();

    bool isActive();
    void setActive();
    void setNotActive();
    int serverReplicaSubservice();
    int clientReplicaSubservice();

private:
    bool currentState;
    participante *tabelaParticipantes;
    bool *tabelaEstaAtualizada;
    string localHostname;
    string localIpAddress;
    string localMacAddress;
    string localStatus;
    string sessionMode;
    uint64_t latestReplica; // TORNAR ISSO ACESSÁVEL PELA ELEIÇÃO
};

#endif
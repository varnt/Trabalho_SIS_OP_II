#ifndef MANAGEMENT_SUBSERVICE_HPP
#define MANAGEMENT_SUBSERVICE_HPP
#include "../global.hpp"

class MonitoringSubservice
{
public:
    MonitoringSubservice(participante *&tabelaParticipantes, bool *tabelaParticipantesUpdate, string localHostName, string localIpAddress, string localMacAddress, string localStatus, string sessionMode);
    ~MonitoringSubservice();

    bool isActive();
    void setActive();
    void setNotActive();
    int serverMonitoringSubservice();
    int clientMonitoringSubservice();

private:
    bool currentState;
    participante *tabelaParticipantes;
    bool *tabelaEstaAtualizada;
    string localHostname;
    string localIpAddress;
    string localMacAddress;
    string localStatus;
    string sessionMode;
};

#endif
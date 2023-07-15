#ifndef DISCOVERY_SUBSERVICE_HPP
#define DISCOVERY_SUBSERVICE_HPP

#include "../global.cpp"
#include "../common/socketAPI.hpp"
#include "../common/packet.hpp"

//classe DiscoverySubservice
class DiscoverySubservice {
public:

    DiscoverySubservice(participante*& tabelaParticipantes, string localHostname, string localIP, string localMAC, string localStatus);
    ~DiscoverySubservice();
    void setActive();
    void setNotActive();
    int serverDiscoverySubservice();
    int clientDiscoverySubservice();
private:
    static struct participante* tabelaParticipantes;
    int socket;
    bool isActive;
    string localHostname;
    string localIP;
    string localMAC;
    string localStatus;
};

#endif
#ifndef DISCOVERY_SUBSERVICE_HPP
#define DISCOVERY_SUBSERVICE_HPP

#include "../global.hpp"
#include "../common/socketAPI.hpp"
#include "../common/packet.hpp"

//classe DiscoverySubservice
class DiscoverySubservice {
public:

    DiscoverySubservice(string localHostname, string localIP, string localMAC, string localStatus);
    ~DiscoverySubservice();
    void setActive();
    void setNotActive();
    int serverDiscoverySubservice(participante*& tabelaParticipantes);
    int clientDiscoverySubservice();
private:
    int socket;
    bool isActive;
    string localHostname;
    string localIP;
    string localMAC;
    string localStatus;
};

#endif
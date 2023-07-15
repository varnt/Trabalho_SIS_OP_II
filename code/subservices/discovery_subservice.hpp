#include "../global.hpp"
#include "../common/socketAPI.hpp"
#include "../common/packet.hpp"

//classe DiscoverySubservice
class DiscoverySubservice {
public:

    DiscoverySubservice(participante* tabelaParticipantes, char* localHostname, char* localIP, char* localMAC, char* localStatus){
        this->tabelaParticipantes = tabelaParticipantes;
        this->socket = PORTA_DESCOBERTA;
    };
    ~DiscoverySubservice();
    void setActive();
    void setNotActive();
    int serverDiscoverySubservice();
    int clientDiscoverySubservice();
private:
    static participante* tabelaParticipantes;
    int socket;
    bool isActive;
    char* localHostname;
    char* localIP;
    char* localMAC;
    char* localStatus;
};
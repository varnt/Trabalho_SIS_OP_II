#include "../global.hpp"
#include "../handler/socketAPI.hpp"

//classe DiscoverySubservice
class DiscoverySubservice {
public:

    DiscoverySubservice(participante* tabelaParticipantes){
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
};
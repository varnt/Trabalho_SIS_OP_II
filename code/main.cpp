// #include "./subservices/management_subservice.hpp"
// #include "./subservices/monitoring_subservice.hpp"
#include "./subservices/interface_subservice.hpp"
#include "./subservices/discovery_subservice.hpp"
#include "global.hpp"

int main(int argc, char **argv)
{

    // Gets local information
    string localMacAddress = getMacAddress();
    string localStatus = "awaken";
    string localIpAddress = getLocalIpAddress();
    string localHostName = gethostname();
    string sessionMode;
    if (argc >= 2 && strcmp(argv[1], "manager") == 0)
    {
        cout << "Session Mode: Manager" << endl;
        sessionMode = "manager";
    }
    else
    {
        cout << "Função: Client" << endl;
        sessionMode = "client";
    }

    // Critical section for Table declaration
    participante *tabelaParticipantes = nullptr; // inicio da lista

    // Critical section for TableUpdate declaration
    cout << "\n";
    novoParticipante(tabelaParticipantes, localHostName, localIpAddress, localMacAddress, localStatus);

    extern bool tabelaParticipantesUpdate;

    InterfaceSubservice interface(tabelaParticipantes, &tabelaParticipantesUpdate);

    // printList(tabelaParticipantes);

    DiscoverySubservice discovery_obj(localHostName, localIpAddress, localMacAddress, localStatus);
    if (sessionMode == "manager")
    {
        interface.setActive();
        interface.updateServerScreen();
        discovery_obj.serverDiscoverySubservice(tabelaParticipantes);
    }
    else if (sessionMode == "client")
    {
        interface.setActive();
        interface.updateClientScreen();
        discovery_obj.clientDiscoverySubservice();
    }

    return 0;
}

//#include "./subservices/management_subservice.hpp"
//#include "./subservices/monitoring_subservice.hpp"
//#include "./subservices/interface_subservice.hpp"
#include "./subservices/discovery_subservice.hpp"
#include "global.hpp"

int main(int argc, char **argv)
{ 
    if(argc >= 2 && strcmp(argv[1], "manager") == 0){
        cout << "Session Mode: Manager" << endl;
    }
    else if (argc >= 0)
    {
        cout << "Função: Client" << endl;
    }
    string sessionMode = argv[1];
    string localMacAddress = getMacAddress();
    string localStatus = "awaken"; 
    string localIpAddress= getLocalIpAddress();
    string localHostName = gethostname();
    cout<<"\n";
    novoParticipante(tabelaParticipantes, localHostName, localIpAddress, localMacAddress, localStatus);
    printList(tabelaParticipantes);
    
    //TESTES:
    /*novoParticipante(tabelaParticipantes, "Adamastor", localIpAddress, "A", localStatus);
    novoParticipante(tabelaParticipantes, "Beatriz", localIpAddress, localMacAddress, localStatus);
    printList(tabelaParticipantes);
    cout<<"\n";
    excluirParticipante(tabelaParticipantes,"A");
    printList(tabelaParticipantes);
    setStatusTabela(tabelaParticipantes,"Batatinha","CEBOLINHA");
    cout<<"\n";
    excluirParticipante(tabelaParticipantes,"A");
    */
    
    return 0;
}
/*int main(int argc, char **argv)
{
    string hello = "Iniciando sistema Marco Polo de otimização de recursos na rede local";
    //[parametros da maquina local
    string localMacAddress = getMacAddress();
    localStatus = "awaken"; 
    string localIpAddress= getLocalIpAddress();
    string localHostName = gethostname();

    //novoParticipante(tabelaParticipantes, localHostName, localIpAddress, localMacAddress, localStatus);
    //novoParticipante(tabelaParticipantes, localHostName, localIpAddress, localMacAddress, localStatus);
    // Impressão da lista encadeada
    
    
    //definir variaveis globais do semaforo

    //]
    //[inicializar parametros web socket aqui
    //porta padrao é 4000
    //]

    //[inicializar thread aqui
    // pthread create (para cada função monitoramento ....)
    //]


    // Terminal talk
    cout << hello;
    cout << endl;
    ///printList(tabelaParticipantes);


    //disparar thread da funcao de descoberta
   ///descoberta();
   //disparar thread da função de monitoramento

   ///monitoring_subservice();

    //thread( monitoring_subservice,)

   
    
    
   
    return 0;
}

            */
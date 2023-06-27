#include "global.h" //arquivo de variaveis globais
#include "monitoring_subservice.cpp"
#include "discovery_subservice.cpp"
#include "management_subservice.cpp"
#include "interface_subservice.cpp"
#include "adminTabela.cpp"


int main(int argc, char **argv)
{
    string hello = "Iniciando sistema Marco Polo de otimização de recursos na rede local";
    //[parametros da maquina local
    string localMacAddress = getMacAddress();
    localStatus = "awaken"; 
    string localIpAddress= getLocalIpAddress();
    string localHostName = gethostname();
    novoParticipante(tabelaParticipantes, localHostName, localIpAddress, localMacAddress, localStatus);
    novoParticipante(tabelaParticipantes, localHostName, localIpAddress, localMacAddress, localStatus);
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
    printList(tabelaParticipantes);


    //disparar thread da funcao de descoberta
   descoberta();
   //disparar thread da função de monitoramento

   monitoring_subservice();

    //thread( monitoring_subservice,)

   
    
    
   
    return 0;
}

            
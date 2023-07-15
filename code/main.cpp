//#include "global.h" //arquivo de variaveis globais
//#include "monitoring_subservice.cpp"
//#include "discovery_subservice.cpp"
//#include "management_subservice.cpp"
//#include "interface_subservice.cpp"
#include "global.hpp"

int main(int argc, char **argv)
{ 
    string localMacAddress = getMacAddress();
    string localStatus = "awaken"; 
    string localIpAddress= getLocalIpAddress();
    string localHostName = gethostname();

    cout<<"\n";
    novoParticipante(tabelaParticipantes, localHostName, localIpAddress, localMacAddress, localStatus);
    printList(tabelaParticipantes);

   //Separar ações entre user e manager
   if (sessionMode == "manager") {

    //Abrir threads e chamar subserviços como manager

   } else if (sessionMode == "client") {

    //Abrir threads e chamar subserviços como user

    } else {
         cout << "userType error in main.cpp";

    
    return 0;
}

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
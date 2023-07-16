#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <chrono> //for timeouts

#include <thread> //for threads

using namespace std;

#define PORTA_DESCOBERTA 4000 //subservico Descoberta
#define PORTA_MONITORAMENTO 4001 //subservico Monitoramento
#define PORTA_GERENCIA 4002 //subservico Gerencia
#define GLOBAL_BROADCAST_ADD "255.255.255.255"
//NOTE TO SELF: INTERFACE NAO TEM PORTA




// Definição da estrutura do nó da lista encadeada
struct participante {
    string hostname;
    string ip_address;
    string mac_address;
    string status;
    participante* next;
};

extern string localStatus; //pode ser bool
extern string sessionMode; //"client" ou "manager"
extern string modo; //MANDATO OU ELEICAO
extern string gerenteHostname; //nome do gerente atual setado na descoberta no modo eleição (ou primeira vez)
extern participante* tabelaParticipantes;

string getLocalIpAddress();
string gethostname();
string getMacAddress();
void novoParticipante(participante*& tabelaParticipantes, string hostname, string ip_address, string mac_address, string status);
void printList(participante* tabelaParticipantes);
void excluirParticipante(participante*& tabelaParticipantes, std::string mac_address);
bool setStatusTabela(participante*& tabelaParticipantes, std::string ip_address,std::string status);

bool estaNaTabela(participante*& tabelaParticipantes, std::string mac_address);

#endif
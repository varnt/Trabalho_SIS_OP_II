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
#include <mutex> // race condition

using namespace std;

#define PORTA_DESCOBERTA 8000 //subservico Descoberta
#define PORTA_DESCOBERTA_CLIENTE 3000 //subservico Descoberta
#define PORTA_MONITORAMENTO 8001 //subservico Monitoramento
#define PORTA_MONITORAMENTO_CLIENTE 3001 //subservico Monitoramento
#define PORTA_GERENCIA 8002 //subservico Gerencia
#define PORTA_GERENCIA_CLIENTE 3002 //subservico Gerencia
#define PORTA_EXIT 3003 // porta para saida
#define GLOBAL_BROADCAST_ADD "255.255.255.255"
//NOTE TO SELF: INTERFACE NAO TEM PORTA


extern mutex mtx, table_mtx;

// Definição da estrutura do nó da lista encadeada
struct participante {
    string hostname;
    string ip_address;
    string mac_address;
    string status;
    participante* next;
};

string getLocalIpAddress();
string gethostname();
string getMacAddress();
void novoParticipante(participante*& tabelaParticipantes, string hostname, string ip_address, string mac_address, string status);
void printList(participante* tabelaParticipantes);
void excluirParticipante(participante*& tabelaParticipantes, std::string mac_address);
bool setStatusTabela(participante*& tabelaParticipantes, std::string ip_address,std::string status);

bool estaNaTabela(participante*& tabelaParticipantes, std::string mac_address);

#endif


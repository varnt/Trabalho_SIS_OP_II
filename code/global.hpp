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

# include <iomanip> // for setw and setfill

using namespace std;

#define PORTA_DESCOBERTA 20000 //subservico Descoberta
#define PORTA_DESCOBERTA_CLIENTE 20010 //subservico Descoberta
#define PORTA_MONITORAMENTO 20001 //subservico Monitoramento
#define PORTA_MONITORAMENTO_CLIENTE 20011 //subservico Monitoramento
#define PORTA_GERENCIA 20002 //subservico Gerencia
#define PORTA_GERENCIA_CLIENTE 20012 //subservico Gerencia
#define PORTA_REPLICA 20003 //subservico Replica
#define PORTA_REPLICA_CLIENTE 20013 //subservico Replica
#define PORTA_EXIT 20066 // porta para saida
#define PORTA_ELEICAO 20004 //subservico Eleicao
#define PORTA_ELEICAO_CLIENTE 20014 //subservico Eleicao

#define PORTA_NOVO_LIDER 20033 //subservico Novo Lider

#define GLOBAL_BROADCAST_ADD "255.255.255.255"

#define MONITORING_FREQUENCY_SEC 1  // Time between full table status checks
//NOTE TO SELF: INTERFACE NAO TEM PORTA

extern bool isRunning;

extern mutex mtx, table_mtx;
extern string MANAGER_IP_ADDRESS;
extern string sessionMode;
extern int self_id;

extern string replica_status, monitoring_status, discovery_status, interface_status;

extern bool isElectionPeriod;

// Definição da estrutura do nó da lista encadeada
struct participante {
    int id;
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
void novoParticipanteID(participante *&tabelaParticipantes, int id, std::string hostname, std::string ip_address, std::string mac_address, std::string status);
void printList(participante* tabelaParticipantes);
void excluirParticipante(participante*& tabelaParticipantes, std::string mac_address);
bool setStatusTabela(participante*& tabelaParticipantes, std::string mac_address,std::string status, int id);

bool estaNaTabela(participante*& tabelaParticipantes, std::string mac_address);

#endif


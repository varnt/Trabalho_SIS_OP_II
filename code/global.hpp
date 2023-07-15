#pragma once

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





using namespace std;

#define PORTA_DESCOBERTA 4000 //subservico Descoberta
#define PORTA_MONITORAMENTO 4001 //subservico Monitoramento
#define PORTA_GERENCIA 4002 //subservico Gerencia
#define GLOBAL_BROADCAST_ADD "255.255.255.255"
//NOTE TO SELF: INTERFACE NAO TEM PORTA

string localStatus; //pode ser bool
string sessionMode; //"manager" ou "client"
string modo; //MANDATO OU ELEICAO
string gerenteHostname; //nome do gerente atual setado na descoberta no modo eleição (ou primeira vez)
participante* tabelaParticipantes = nullptr; //inicio da lista

// Definição da estrutura do nó da lista encadeada
struct participante {
    string hostname;
    string ip_address;
    string mac_address;
    string status;
    participante* next;
};

extern participante* tabelaParticipantes;
string getLocalIpAddress();
string gethostname();
string getMacAddress();
string getUserType(); //retorna USER ou MANAGER; TODO: implementar

//Variaveis compartilhadas


void novoParticipante(participante*& tabelaParticipantes, string hostname, string ip_address, string mac_address, string status);
void printList(participante* tabelaParticipantes);
void excluirParticipante(participante*& tabelaParticipantes, std::string mac_address);
bool setStatusTabela(participante*& tabelaParticipantes, std::string ip_address,std::string status);

bool estaNaTabela(participante*& tabelaParticipantes, std::string mac_address);



// Definição de estrutura de pacotes
#define HOSTNAME_SIZE 256
#define IP_ADDR_SIZE 16
#define MAC_SIZE 18
#define STATUS_SIZE 1

typedef struct __packet_struct {
    uint sequence_number; //Número de sequência
    uint16_t dest_port; //Porta de destino
    uint16_t src_port; //Porta de origem
    char ip_dest[IP_ADDR_SIZE]; //IP de destino
    char ip_src[IP_ADDR_SIZE]; //IP de origem
    char hostname[HOSTNAME_SIZE]; //Hostname de origem
    char mac_src[MAC_SIZE]; //MAC de destino
    char status[STATUS_SIZE]; //Status
    char message[1]; //Mensagem
} packet_struct;

packet_struct* createPacket(uint sequence_number, uint16_t dest_port, uint16_t src_port, char* ip_dest, char* ip_src, char* hostname, char* mac_src, char* status, char* message);
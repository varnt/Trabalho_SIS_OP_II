#ifndef PACKET_HPP
#define PACKET_HPP

#include "../global.hpp"

// Definição de estrutura de pacotes
#define HOSTNAME_SIZE 256
#define IP_ADDR_SIZE 16
#define MAC_SIZE 18
#define STATUS_SIZE 16

//Definição de códigos de mensagem
#define SYN 0
#define SYN_ACK 1
#define ACK 2
#define EXIT_MSG 3

// Definição de estrutura de pacotes
typedef struct __packet_struct {
    uint sequence_number; //Número de sequência
    uint16_t dest_port; //Porta de destino
    uint16_t src_port; //Porta de origem
    char ip_dest[IP_ADDR_SIZE]; //IP de destino
    char ip_src[IP_ADDR_SIZE]; //IP de origem
    char hostname[HOSTNAME_SIZE]; //Hostname de origem
    char mac_src[MAC_SIZE]; //MAC de destino
    char status[STATUS_SIZE]; //Status
    int message; //Mensagem (0 = SYN, 1 = SYN-ACK, 2 = ACK
} packet_struct;

typedef struct __replica_struct {
    uint sequence_number; //Número de sequência
    uint16_t dest_port; //Porta de destino
    uint16_t src_port; //Porta de origem
    char ip_src[IP_ADDR_SIZE]; //IP de origem
    int part_id; //ID do participante
    char part_hostname[HOSTNAME_SIZE]; //Hostname do participante
    char part_ip[IP_ADDR_SIZE]; //IP do participante
    char part_mac[MAC_SIZE]; //MAC do participante
    char part_status[STATUS_SIZE]; //Status do participante
    uint64_t timestamp; //Timestamp for the replica
    int message; //Mensagem (0 = SYN, 1 = SYN-ACK, 2 = ACK
} replica_struct;


/*
Function to create a packet
It must receive string parameters for non-int, but it will convert them to char* arrays internally
*/
packet_struct createPacket(uint sequence_number, uint16_t dest_port, uint16_t src_port, string ip_dest, string ip_src, string hostname, string mac_src, string status, int message);

replica_struct createReplicaPkt(uint sequence_number, uint16_t dest_port, uint16_t src_port, string ip_src, int part_id, string part_hostname, string part_ip, string part_mac, string part_status, uint64_t timestamp, int message);
#endif

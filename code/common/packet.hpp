
// Definição de estrutura de pacotes
#define HOSTNAME_SIZE 256
#define IP_ADDR_SIZE 16
#define MAC_SIZE 18
#define STATUS_SIZE 1

//Definição de códigos de mensagem
#define SYN 0
#define SYN_ACK 1
#define ACK 2

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

packet_struct createPacket(uint sequence_number, uint16_t dest_port, uint16_t src_port, char* ip_dest, char* ip_src, char* hostname, char* mac_src, char* status,int message);
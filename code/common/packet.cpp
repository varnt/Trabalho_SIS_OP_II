#include "packet.hpp"

// Função para criar um pacote
packet_struct createPacket(uint sequence_number, uint16_t dest_port, uint16_t src_port, string ip_dest, string ip_src, string hostname, string mac_src, string status, int message) {
    packet_struct packet;
    packet.sequence_number = sequence_number;
    packet.dest_port = dest_port;
    packet.src_port = src_port;
    strcpy(packet.ip_dest, ip_dest);
    strcpy(packet.ip_src, ip_src);
    strcpy(packet.hostname, hostname);
    strcpy(packet.mac_src, mac_src);
    strcpy(packet.status, status);
    packet.message = message;
    return packet;
}
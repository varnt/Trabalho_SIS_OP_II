#include "packet.hpp"

// Função para criar um pacote
packet_struct createPacket(uint sequence_number, uint16_t dest_port, uint16_t src_port, string ip_dest, string ip_src, string hostname, string mac_src, string status, int message) {
    packet_struct packet;
    packet.sequence_number = sequence_number;
    packet.dest_port = dest_port;
    packet.src_port = src_port;
    strcpy(packet.ip_dest, (char*)ip_dest.c_str());
    strcpy(packet.ip_src, (char*)ip_src.c_str());
    strcpy(packet.hostname, (char*)hostname.c_str());
    strcpy(packet.mac_src, (char*)mac_src.c_str());
    strcpy(packet.status, (char*)status.c_str());
    packet.message = message;
    return packet;
}

replica_struct createReplicaPkt(uint sequence_number, uint16_t dest_port, uint16_t src_port, string ip_src, int part_id, string part_hostname, string part_ip, string part_mac, string part_status, uint64_t timestamp, int message) {
    replica_struct packet;
    packet.sequence_number = sequence_number;
    packet.dest_port = dest_port;
    packet.src_port = src_port;
    strcpy(packet.ip_src, (char*)ip_src.c_str());
    packet.part_id = part_id;
    strcpy(packet.part_hostname, (char*)part_hostname.c_str());
    strcpy(packet.part_ip, (char*)part_ip.c_str());
    strcpy(packet.part_mac, (char*)part_mac.c_str());
    strcpy(packet.part_status, (char*)part_status.c_str());
    packet.timestamp = timestamp;
    packet.message = message;
    return packet;
}
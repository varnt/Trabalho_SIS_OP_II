#ifndef SOCKETAPI_HPP
#define SOCKETAPI_HPP

#include "../global.hpp"
#include "../common/packet.hpp"

class SocketAPI
{
public:
    SocketAPI(int port, string sessionMode);
    SocketAPI(int port, string serverAddr, string sessionMode);
    ~SocketAPI();

    int getSocketfd() { return this->socketfd; };

    //TODO: implement the subprocesses interfaces with the socket
    int sendPacket(packet_struct *packet, string destIP, uint16_t destPort);
    int listenSocket(packet_struct *packet);

private:
    int port;           //port number: must be one of the defined ones in global.hpp
    int socketfd;       //socket file descriptor
    string sessionMode; //session mode: "client" or "manager", used to define the socket behavior
    int createSocket();
    sockaddr_in serverAddr;
};

#endif
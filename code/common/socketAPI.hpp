#pragma once

#include "../global.hpp"
#include "../common/packet.hpp"

class SocketAPI {
    public:
        SocketAPI(int port, string sessionMode);
        SocketAPI(int port, string serverAddr, string sessionMode);
        ~SocketAPI();

        int getSocketfd() { return this->socketfd; };

        //TODO: implement the subprocesses interfaces with the socket
        int sendPacket(packet_struct* packet, char* destIP, uint16_t destPort);
        int listenSocket(packet_struct* packet);


        


    private:
        int port; //port number: must be one of the defined ones in global.hpp
        int socketfd; //socket file descriptor
        string sessionMode; //session mode: "client" or "manager", used to define the socket behavior
        string serverAddr; //server address: used only in client mode, to define the server address

        int createSocket();
        
};

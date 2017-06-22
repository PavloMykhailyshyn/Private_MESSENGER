#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define BUFF_SIZE 1024

class clientsocket
{
    static clientsocket * instance_;


    clientsocket(std::string = localhost_, std::int32_t = port_);


    static const std::string localhost_; // = "127.0.0.1";
    static const std::int32_t port_ = 1111;
    std::int32_t client_socket_desc_;
    sockaddr_in server_data_;

    char buff[BUFF_SIZE] = { 0 };

    /*bool SendAll(const char *, std::int32_t) const;
    bool RecvAll(char *, int) const;
    bool SendInt(std::int32_t) const;
    bool GetInt(std::int32_t&) const;*/


public:
    clientsocket(const clientsocket&)               = delete;
    clientsocket operator = (const clientsocket&)   = delete;

    //enum _PACKET_TYPE_ { MSG = 1, STATE, MSG_TO_ALL, MSG_TO_SMB };
    //enum _STATE_ { ONLINE = 1, OFFLINE, BUSY };

    bool Connect();

    /*bool SendString(const std::string&) const;
    bool SendMsgToSmb(const std::string&) const;
    bool SendMsgToAll(const std::string&) const;
    bool SendState(_STATE_) const;*/

    /*bool SendPacketType(PACKET_TYPE) const;
    bool GetPacketType(PACKET_TYPE &) const;*/

    bool SendString(const std::string&) const;
    bool GetString(std::string&);

    void SetIpv4Addr(std::string);
    void SetPort(std::int32_t);

    std::int32_t GetSocketDesc() const;

    static clientsocket * GetInstance()
    {
        if (!instance_)
        {
            instance_ = new clientsocket;
        }
        return instance_;
    }

    static void ResetInstance()
    {
        if (instance_)
        {
            delete instance_;
        }
        instance_ = nullptr;
    }

    ~clientsocket();
};

extern clientsocket * client_ptr;

#endif // CLIENTSOCKET_H

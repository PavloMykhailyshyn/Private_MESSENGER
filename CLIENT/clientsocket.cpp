#include "clientsocket.h"

clientsocket * clientsocket::instance_ = nullptr;
const std::string clientsocket::localhost_ = "127.0.0.1";

clientsocket::clientsocket(std::string ip_addr, std::int32_t port)
{
    client_socket_desc_ = socket(AF_INET , SOCK_STREAM , 0);
    if (client_socket_desc_ == -1)
    {
        std::cerr << "Could not create socket\n";
    }
    std::cout << client_socket_desc_ << std::endl;

    server_data_.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    server_data_.sin_family = AF_INET;
    server_data_.sin_port = htons( port );
}

void clientsocket::SetIpv4Addr(std::string ip_addr)
{
    server_data_.sin_addr.s_addr = inet_addr(ip_addr.c_str());
}

void clientsocket::SetPort(std::int32_t port)
{
    server_data_.sin_port = htons( port );
}

bool clientsocket::Connect()
{
    if (connect(client_socket_desc_, (const sockaddr *)&server_data_, sizeof(server_data_)) < 0)
    {
        perror("connect()");
        std::cerr << "ERROR: Failed to Connect!\n";
        return false;
    }
    std::cout << "Connected!" << std::endl;
    return true;
}

/*bool clientsocket::RecvAll(char * data, std::int32_t total_bytes) const
{
    std::int32_t bytes_received = 0;
    while (bytes_received < total_bytes)
    {
        std::int32_t return_check = recv(client_socket_desc_, data, total_bytes - bytes_received, NULL);
        if (return_check < 0)
            return false;
        bytes_received += return_check;
    }
    return true;
}

bool clientsocket::SendAll(const char * data, std::int32_t total_bytes) const
{
    std::int32_t bytes_sent = 0;
    while (bytes_sent < total_bytes)
    {
        std::int32_t return_check = send(client_socket_desc_, data + bytes_sent, total_bytes - bytes_sent, NULL);
        if (return_check < 0)
            return false;
        bytes_sent += return_check;
    }
    return true;
}

bool clientsocket::SendInt(std::int32_t int_bytes) const
{
    int_bytes = htonl(int_bytes);
    return SendAll((char*)&int_bytes, sizeof(int_bytes));
}

bool clientsocket::GetInt(std::int32_t & int_bytes) const
{
    if (!RecvAll((char*)&int_bytes, sizeof(int_bytes)))
        return false;
    int_bytes = ntohl(int_bytes);
    return true;
}

bool clientsocket::SendString(const std::string & str) const
{
    std::int32_t buffer_length = str.size();
    if (!SendInt(buffer_length))
        return false;
    return SendAll((char*)str.c_str(), buffer_length);
}

bool clientsocket::GetString(std::string & str) const
{
    std::int32_t buffer_length;
    if (!GetInt(buffer_length))
        return false;
    char * buffer = new char[buffer_length + 1];
    buffer[buffer_length] = '\0';
    if (!RecvAll(buffer, buffer_length))
    {
        delete[] buffer;
        return false;
    }
    str = buffer;
    delete[] buffer;
    return true;
}

bool clientsocket::SendState(_STATE_ state) const
{
    return SendInt(state);
}

bool clientsocket::GetPacketType(PACKET_TYPE & packet_type) const
{

}*/

bool clientsocket::SendString(const std::string & str) const
{
    if (-1 == send(client_socket_desc_, str.c_str(), str.size(), 0))
        return false;
    return true;
}

bool clientsocket::GetString(std::string & str)
{
    if (-1 == recv(client_socket_desc_, buff, BUFF_SIZE, 0))
        return false;
    std::string temp_str(buff);
    str = temp_str;
    memset(buff, 0, BUFF_SIZE);
    return true;
}


std::int32_t clientsocket::GetSocketDesc() const
{
    return client_socket_desc_;
}


clientsocket::~clientsocket()
{
    close(client_socket_desc_);
    std::cout << "Socket successfully closed" << std::endl;
}

clientsocket * client_ptr = clientsocket::GetInstance();

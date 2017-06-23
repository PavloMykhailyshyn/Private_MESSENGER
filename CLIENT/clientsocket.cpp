#include "clientsocket.h"

clientsocket * clientsocket::instance_ = nullptr;
const std::string clientsocket::localhost_ = "127.0.0.1";

clientsocket::clientsocket(std::string ip_addr, std::int32_t port)
{
#ifdef QT_DEBUG
    std::cout << "Socket successfully opened" << std::endl;
#endif

    client_socket_desc_ = socket(AF_INET , SOCK_STREAM , 0);
    if (-1 == client_socket_desc_)
    {
#ifdef QT_DEBUG
        std::cerr << "Could not create socket\n";
#endif
    }

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
#ifdef QT_DEBUG
        perror("connect()");
        std::cerr << "ERROR: Failed to Connect!\n";
#endif
        return false;
    }
#ifdef QT_DEBUG
    std::cout << "Connected!" << std::endl;
#endif
    return true;
}

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
#ifdef QT_DEBUG
    std::cout << "Socket successfully closed" << std::endl;
#endif
}

clientsocket * client_ptr = clientsocket::GetInstance();

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
namespace server
{

    class ServerHTTP
    {
        int sock = 0; // nr socketului
        int port = 0; // portul

    public:
                void writeEndpointAddress(sockaddr_in &endpoint, int port_)
        {
            endpoint.sin_family = AF_INET;
            endpoint.sin_port = htons(port_);
            endpoint.sin_addr.s_addr = inet_addr("127.0.0.1");
            memset(endpoint.sin_zero, 0, sizeof(endpoint.sin_zero)); // zero out
        }
        // constructor initial
        ServerHTTP(int port_) : port(port_)
        {

            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // creare socket
            if (sock == -1)
            {
                throw std::runtime_error("Eroare la crearea socketului");
            }
            sockaddr_in endpoint{};
            writeEndpointAddress(endpoint, this->port);

            if (bind(sock, (sockaddr *)&endpoint, sizeof(endpoint)) == -1)
            {
                throw std::runtime_error(std::string("Eroare la bind: ") + strerror(errno) + "\n");
            }
        }

        int getSock()
        {
            if (sock != 0 || sock != -1)
            {
                return sock;
            }
            else
            {
                return -1;
            }
        }
    };
}

#endif
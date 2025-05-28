#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <cstring>

namespace client
{
    class Client
    {
        int sock = 0;

    public:
        Client()
        {
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // creare socket
            if (sock == -1)
            {
                throw std::runtime_error("Eroare la crearea socketului");
            }
        }

        int connectToServer(sockaddr *addr)
        {
            if (connect(sock, addr, sizeof(sockaddr_in)) == -1)
            {
                throw std::runtime_error(std::string("Eroare la connect: ") + strerror(errno));
            }
            return 0;
        }

        int getSock()
        {
            return sock;
        }
    };
}

#endif
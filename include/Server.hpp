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
        int sock = 0; // nr socketului - defapt a fisierului de descriere a socketului
        int port = 0; // portul
        const int conexiuniSimultane = 1;
        // functie care seteaza parametrii adresei
        void writeEndpointAddress(sockaddr_in &endpoint, int port_)
        {
            endpoint.sin_family = AF_INET;                     // familia - IPv4
            endpoint.sin_port = htons(port_);                  // portul
            endpoint.sin_addr.s_addr = inet_addr("127.0.0.1"); // adresa IP
            // un fel de padding pentru ca trebuie sockaddr_in convertit in sockaddr
            // si nu au aceleasi buffere
            memset(endpoint.sin_zero, 0, sizeof(endpoint.sin_zero));
        }

    public:
        // constructor initial
        ServerHTTP(int port_) : port(port_)
        {

            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // creare socket
            if (sock == -1)
            {
                throw std::runtime_error("Eroare la crearea socketului");
            }
            sockaddr_in endpoint{};                     // adresa socket
            writeEndpointAddress(endpoint, this->port); // setarea adresei
            // punerea adresei pe socket
            if (bind(sock, (sockaddr *)&endpoint, sizeof(endpoint)) == -1)
            {
                close(sock);
                throw std::runtime_error(std::string("Eroare la bind: ") + strerror(errno) + "\n");
            }
            else
            {
                std::cout << "bind is ok" << std::endl;
            }
            // permiterea ascultarii serverului pentru cereri
            if (listen(sock, conexiuniSimultane) == -1)
            {
                throw std::runtime_error(std::string("Eroare la listen: ") + strerror(errno) + "\n");
            }
            else
            {
                std::cout << "Inceperea ascultarii e ok" << std::endl;
            }
        }

        ~ServerHTTP()
        {
            if (sock != -1)
            {
                close(sock);
            }
        }

        int pornesteServer()
        {
            int socketDuplicat;
            socketDuplicat = accept(sock, NULL, NULL); // functia asta blocheaza
                                                       // rularea programului pana la acceptare unei conexiuni
            if (socketDuplicat == -1)
            {
                std::cerr << "Eroare la accept" << strerror(errno) << std::endl;
            }
            else
            {
                std::cout << "S-a acceptat conexiunea!" << std::endl;
            }
            return socketDuplicat;
        }

        int primesteDate(int socket, char *buffer)
        {
            int byteCount = recv(socket, buffer, 200, 0);
            if (byteCount == -1)
            {
                std::cerr << "Eroare la accept" << strerror(errno) << std::endl;
            }
            return byteCount;
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
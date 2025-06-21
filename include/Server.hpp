#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sstream>
#include <fstream>
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

        void parseazaCerere(std::string cerere, std::string &method, std::string &path)
        {
            std::istringstream iss(cerere);
            iss >> method >> path;
        }

        std::string getContentType(const std::string &path)
        {
            // Extrage extensia
            size_t punct = path.find_last_of(".");
            if (punct == std::string::npos)
                return "text/plain"; // fără extensie

            std::string extensie = path.substr(punct + 1);

            if (extensie == "html" || extensie == "htm")
                return "text/html";
            if (extensie == "css")
                return "text/css";
            if (extensie == "js")
                return "application/javascript";
            if (extensie == "png")
                return "image/png";
            if (extensie == "jpg" || extensie == "jpeg")
                return "image/jpeg";
            if (extensie == "gif")
                return "image/gif";
            if (extensie == "svg")
                return "image/svg+xml";
            if (extensie == "ico")
                return "image/x-icon";
            if (extensie == "json")
                return "application/json";
            if (extensie == "pdf")
                return "application/pdf";
            if (extensie == "txt")
                return "text/plain";
            if (extensie == "xml")
                return "application/xml";
            if (extensie == "woff")
                return "font/woff";
            if (extensie == "woff2")
                return "font/woff2";
            if (extensie == "ttf")
                return "font/ttf";

            return "application/octet-stream"; // fallback pentru necunoscute
        }

        void rezolvaGET(std::string cerere, std::string path, int socket)
        {
            std::cout << "Am ajuns in rezolvaGET()" << std::endl;
            if (path == "/")
            {
                path = "index.html"; // daca e prima cerere se serveste prima pagina
            }
            else
            {
                if (path[0] == '/')
                {
                    path = path.substr(1);
                }
            }
            std::ifstream file(path, std::ios::binary); // se deschide fisierul in continut binar
            if (!file)
            {
                std::string raspuns = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n404 - Not Found";
                send(socket, raspuns.c_str(), raspuns.size(), 0);
                close(socket);
                return;
            }
            else
            {
                std::ostringstream ss;
                ss << file.rdbuf();
                std::string body = ss.str();
                std::string header = "HTTP/1.1 200 OK";
                std::string content_type = "Content-Type: " + getContentType(path);
                std::string content_length = "Content-Length: " + std::to_string(body.length());
                header = header + "\r\n" + content_type + "\r\n" + content_length + "\r\n" + "Connection: close" + "\r\n";
                std::string response = header + "\r\n" + body;
                send(socket, response.c_str(), response.size(), 0);
                close(socket);
                return;
            }
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

        void proceseazaCerere(int socket)
        {
            char buffer[1024];
            int byteCount = recv(socket, buffer, 1024, 0); // se primeste cererea de la browser, se salveaza intr-un buffer
            if (byteCount == -1)
            {
                std::cerr << "Eroare la accept" << strerror(errno) << std::endl;
                return;
            }
            std::string method = "MetodaImplicita", path;
            std::string cerere(buffer, byteCount);
            parseazaCerere(cerere, method, path); // se salveaza metoda HTTP (in general o sa fie doar GET) si path-ul, respectiv ce fisier se vrea de pe server
            std::cout << method << std::endl;
            if (method == "GET")
            {
                rezolvaGET(cerere, path, socket); // in aceasta metoda se apeleaza send()
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
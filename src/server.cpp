#include <iostream>
#include "Server.hpp"

int main()
{
    int port = 55555;
    char buffer[200];
    server::ServerHTTP serverhtpp(port);
    int socketServer = serverhtpp.pornesteServer();
    // std::cout << serverhtpp.getSock() << "\n";
    int byteCount = serverhtpp.primesteDate(socketServer, buffer);
    std::cout << "Ai primit mesajul: " << buffer << std::endl;
}
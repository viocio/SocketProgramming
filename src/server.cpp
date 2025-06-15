#include <iostream>
#include "Server.hpp"

int main()
{
    int port = 55555;
    char buffer[200];
    server::ServerHTTP serverhtpp(port);
    int socketServer = serverhtpp.pornesteServer();

    serverhtpp.proceseazaCerere(socketServer);
    // std::cout << serverhtpp.getSock() << "\n";
    // while (true)
    // {
    //     int byteCount = serverhtpp.primesteDate(socketServer, buffer);
    //     if (byteCount <= 0) // clientul a închis sau eroare
    //     {
    //         std::cout << "Clientul a închis conexiunea sau a apărut o eroare." << std::endl;
    //         break;
    //     }

    //     buffer[byteCount] = '\0'; // adaugă terminator de șir

    //     if (strcmp(buffer, "exit") == 0)
    //     {
    //         std::cout << "Clientul a trimis 'exit'. Închidem conexiunea." << std::endl;
    //         break;
    //     }

    //     std::cout << "Ai primit mesajul: " << buffer << std::endl;
    // }

    return 0;
}
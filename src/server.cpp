#include <iostream>
#include "Server.hpp"

int main()
{
    int port = 55555;
    server::ServerHTTP serverhtpp(port); // se creeaza serverul: se deschide socket, se binduieste, se instantieaza pentru liste

    while (true)
    {
        int socketServer = serverhtpp.pornesteServer(); // se porneste server, se intoarce acel socket duplicat, se dar doar dupa ce se accepta o conexiune
                                                        // se blocheaza rularea programului de metoda pornesteServer()
        if (socketServer == -1)
        {
            std::cout << "A aparut o eroare" << std::endl;
        }
        else
        {
            serverhtpp.proceseazaCerere(socketServer); // se proceseaza cererea de la browser - probabil ar trebui sa fie intr-o bucla
        }
        }
    return 0;
}
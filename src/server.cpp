#include <iostream>
#include "Server.hpp"

int main()
{
    int port = 55555;
    server::ServerHTTP serverhtpp(port);
    serverhtpp.pornesteServer();
    std::cout << serverhtpp.getSock() << "\n";
}
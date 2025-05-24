#include <iostream>
#include "Server.hpp"

int main()
{
    int port = 55555;
    server::ServerHTTP serverhtpp(port);
    std::cout << serverhtpp.getSock() << "\n";
}
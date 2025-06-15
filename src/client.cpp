#include <iostream>
#include "Client.hpp"
#include <arpa/inet.h>
#include <string.h>

void writeEndpointAddress(sockaddr_in &endpoint, int port_) // acceasi functie ca si in Server.hpp
{
    endpoint.sin_family = AF_INET;                     // familia - IPv4
    endpoint.sin_port = htons(port_);                  // portul
    endpoint.sin_addr.s_addr = inet_addr("127.0.0.1"); // adresa IP
    memset(endpoint.sin_zero, 0, sizeof(endpoint.sin_zero));
}

int main()
{
    client::Client clientHTTP;
    // std::cout << clientHTTP.getSock() << std::endl;
    sockaddr_in adresaServer{};
    writeEndpointAddress(adresaServer, 55555);
    std::cout << clientHTTP.connectToServer((sockaddr *)&adresaServer) << std::endl;
    char buffer[200];
    printf("Introdu mesajul: ");
    std::cin.getline(buffer, 200);
    int byteCount = clientHTTP.trimiteDate(clientHTTP.getSock(), buffer);
}
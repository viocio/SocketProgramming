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
    char buffer[200] = "GET /";

    clientHTTP.trimiteDate(clientHTTP.getSock(), buffer);

    // while (true)
    // {
    //     std::cout << "Trimite mesaj: ";
    //     std::cin.getline(buffer, 200);

    //     if (strcmp(buffer, "exit") == 0)
    //     {
    //         break; // închidem clientul dacă scriem "exit"
    //     }

    //     int byteCount = clientHTTP.trimiteDate(clientHTTP.getSock(), buffer);
    //     if (byteCount == -1)
    //     {
    //         std::cout << "Nu s-a putut transmite ultimul mesaj!" << std::endl;
    //         break;
    //     }
    // }

    return 0;
}
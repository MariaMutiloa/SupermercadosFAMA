
#include <iostream>
#include <cstring>
#include <cerrno>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int iniciarServidor(const std::string& puerto) {
    // Inicializar Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock." << std::endl;
        return -1;
    }

    // Configurar los detalles del servidor
    std::string serverAddress = "0.0.0.0";  // Escuchar en todas las interfaces de red

    // Crear el socket
    SOCKET socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // Permitir la reutilización del puerto
    int yes = 1;
    if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes)) == SOCKET_ERROR) {
        std::cerr << "Error al configurar el socket: " << WSAGetLastError() << std::endl;
        closesocket(socketFD);
        WSACleanup();
        return -1;
    }

    // Obtener información del servidor
    struct sockaddr_in serverInfo;
    memset(&serverInfo, 0, sizeof(serverInfo));
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr(serverAddress.c_str());
    serverInfo.sin_port = htons(std::stoi(puerto));

    // Vincular el socket a la dirección y puerto del servidor
    if (bind(socketFD, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) == SOCKET_ERROR) {
        std::cerr << "Error al vincular el socket: " << WSAGetLastError() << std::endl;
        closesocket(socketFD);
        WSACleanup();
        return -1;
    }

    // Escuchar conexiones entrantes
    if (listen(socketFD, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error al escuchar conexiones entrantes: " << WSAGetLastError() << std::endl;
        closesocket(socketFD);
        WSACleanup();
        return -1;
    }

    return socketFD;
}


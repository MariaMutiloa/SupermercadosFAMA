#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../sqlite3.h"

int main() {
    // Crea un socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error al crear el socket." << std::endl;
        return 1;
    }

    //Dirección del servidor
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Puerto del servidor
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //Conectar socket a la dirección del servidor
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Error al enlazar el socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 1) == -1) {
        std::cerr << "Error al escuchar conexiones entrantes." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Esperando conexiones entrantes..." << std::endl;

    // Aceptar conexión entrante
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
    if (clientSocket == -1) {
        std::cerr << "Error al aceptar la conexión entrante." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Convertir la dirección IP del cliente a una cadena
    std::string clientIP = inet_ntoa(clientAddress.sin_addr);
    std::cout << "Conexión establecida con el cliente " << clientIP << std::endl;

    // Conectar a la base de datos SQLite
    sqlite3* db;
    int result = sqlite3_open("database.db", &db);
    if (result != SQLITE_OK) {
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        close(clientSocket);
        close(serverSocket);
        return 1;
    }

    // Recibir una consulta SQL del cliente
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == -1) {
        std::cerr << "Error al recibir datos del cliente." << std::endl;
        close(clientSocket);
        close(serverSocket);
        sqlite3_close(db);
        return 1;
    }
    buffer[bytesRead] = '\0';
    std::string query(buffer);

    // Ejecutar la consulta en la base de datos
    result = executeQuery(db, query);
    if (result == SQLITE_DONE || result == SQLITE_ROW) {
        std::cout << "Consulta ejecutada con éxito." << std::endl;
    } else {
        std::cerr << "Error al ejecutar la consulta: " << sqlite3_errmsg(db) << std::endl;
    }

    // Enviar el resultado de la consulta al cliente
    std::string response = "Resultado de la consulta...";
    if (send(clientSocket, response.c_str(), response.length(), 0) == -1) {
        std::cerr << "Error al enviar datos al cliente." << std::endl;
        close(clientSocket);
        close(serverSocket);
        sqlite3_close(db);
        return 1;
    }

    // Cerrar la conexión y la base de datos
    close(clientSocket);
    close(serverSocket);
    sqlite3_close(db);

    return 0;
}

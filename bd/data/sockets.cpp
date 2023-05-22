#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

sql::Connection* connectToServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock." << std::endl;
        return nullptr;
    }

    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "tu_usuario", "tu_contraseña"); //cambiar por las credenciales de nuestra BD
    } catch (sql::SQLException& e) {
        std::cerr << "Error al conectar a la base de datos: " << e.what() << std::endl;
        WSACleanup();
        return nullptr;
    }

    return con;
}
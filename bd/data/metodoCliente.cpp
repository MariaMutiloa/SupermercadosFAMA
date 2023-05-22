#include <iostream>
#include <cstring>
#include <unistd.h>
#include <winsock2.h>
#include "../sqlite3.h"
#include "metodoCliente.h"
#include "sockets.h"
#include <limits>

void actualizarDatosCliente() {

    int socketFD = connectToServer();
    if (socketFD == -1) {
        // Manejar el error de conexión
        std::cerr << "Error al conectar con el servidor." << std::endl;
        return;
    }
    char dni[20];
    char contrasena[50];
    char opcion[10];
    char sql[200];

    std::cout << "Ingrese su DNI: ";
    std::cin.getline(dni, 20);

    std::cout << "Ingrese su contraseña: ";
    std::cin.getline(contrasena, 50);

    // Enviar DNI y contraseña al servidor
    std::string data = std::string(dni) + " " + std::string(contrasena);
    if (send(socketFD, data.c_str(), data.length(), 0) == -1) {
        std::cerr << "Error al enviar los datos al servidor." << std::endl;
        return;
    }

    // Recibir respuesta del servidor
    char response[256];
    memset(response, 0, sizeof(response));
    if (recv(socketFD, response, sizeof(response), 0) == -1) {
        std::cerr << "Error al recibir la respuesta del servidor." << std::endl;
        return;
    }

    std::cout << response << std::endl;

    // Verificar la existencia del DNI y contraseña en la base de datos
    if (strcmp(response, "OK") != 0) {
        std::cout << "El DNI o contraseña ingresados son incorrectos." << std::endl;
        return;
    }

    // Actualizar la información del cliente
    while (true) {
        std::cout << "¿Qué dato desea actualizar?" << std::endl;
        std::cout << "1. Contraseña" << std::endl;
        std::cout << "2. Dirección de casa" << std::endl;
        std::cout << "3. Correo electrónico" << std::endl;
        std::cout << "4. Salir" << std::endl;
        std::cout << "Ingrese el número de la opción que desee: ";
        std::cin.getline(opcion, 10);

        if (strcmp(opcion, "1") == 0) {
            char nuevaContrasena[50];
            char confirmacion[50];

            std::cout << "Ingrese su nueva contraseña: ";
            std::cin.getline(nuevaContrasena, 50);
            std::cout << "Confirme su nueva contraseña: ";
            std::cin.getline(confirmacion, 50);

            // Verifica que las contraseñas coincidan
            if (strcmp(nuevaContrasena, confirmacion) != 0) {
                std::cout << "Las contraseñas no coinciden, intente nuevamente." << std::endl;
                continue;
            }

            // Actualiza la contraseña
            sprintf(sql, "UPDATE cliente SET contrasena = '%s' WHERE dni = '%s'", nuevaContrasena, dni);
            if (send(socketFD, sql, strlen(sql), 0) == -1) {
                std::cerr << "Error al enviar la consulta al servidor." << std::endl;
                return;
            }

            memset(response, 0, sizeof(response));
            if (recv(socketFD, response, sizeof(response), 0) == -1) {
                std::cerr << "Error al recibir la respuesta del servidor." << std::endl;
                return;
            }

            std::cout << response << std::endl;

        } else if (strcmp(opcion, "2") == 0) {
            char nuevaDireccion[100];

            std::cout << "Ingrese su nueva dirección: ";
            std::cin.getline(nuevaDireccion, 100);

            // Actualiza la dirección en la base de datos
            sprintf(sql, "UPDATE cliente SET Direccion_Domicilio = '%s' WHERE dni = '%s'", nuevaDireccion, dni);
            if (send(socketFD, sql, strlen(sql), 0) == -1) {
                std::cerr << "Error al enviar la consulta al servidor." << std::endl;
                return;
            }

            memset(response, 0, sizeof(response));
            if (recv(socketFD, response, sizeof(response), 0) == -1) {
                std::cerr << "Error al recibir la respuesta del servidor." << std::endl;
                return;
            }

            std::cout << response << std::endl;

        } else if (strcmp(opcion, "3") == 0) {
            char nuevoCorreo[50];

            std::cout << "Ingrese su nuevo correo electrónico: ";
            std::cin.getline(nuevoCorreo, 50);

            // Actualiza el correo electrónico en la base de datos
            sprintf(sql, "UPDATE cliente SET Correo_electronico = '%s' WHERE dni = '%s'", nuevoCorreo, dni);
            if (send(socketFD, sql, strlen(sql), 0) == -1) {
                std::cerr << "Error al enviar la consulta al servidor." << std::endl;
                return;
            }

            memset(response, 0, sizeof(response));
            if (recv(socketFD, response, sizeof(response), 0) == -1) {
                std::cerr << "Error al recibir la respuesta del servidor." << std::endl;
                return;
            }

            std::cout << response << std::endl;

        } else if (strcmp(opcion, "4") == 0) {
            break;
        } else {
            std::cout << "Opción inválida, intente nuevamente." << std::endl;
        }
    }

    closesocket(socketFD);
}

void realizarCompra(int socketFD) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    char respuesta;
    char dni[20];
    int cod_ped, importe, pagado, cantidad_total = 0;
    int cod_prod, cantidad;
    char descripcion[100], nom_clien[100];
    int cantidad_disponible;

    // Conectarse al servidor de la base de datos
    if (connectToServer() == -1) {
        std::cerr << "Error al conectarse al servidor de la base de datos." << std::endl;
        return;
    }

    // Lista los productos disponibles
    std::cout << "Productos disponibles:" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::string sql = "SELECT cod_prod, descripcion, importe, cantidad FROM productos";
    if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
        std::cerr << "Error al enviar la consulta al servidor de la base de datos." << std::endl;
        return;
    }

    char response[256];
    memset(response, 0, sizeof(response));
    if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
        std::cerr << "Error al recibir la respuesta del servidor de la base de datos." << std::endl;
        return;
    }

    std::cout << response;

    // Pedir al usuario que elija un producto y cantidad
    std::cout << "¿Qué producto quiere comprar? (introduzca el código): ";
    std::cin >> cod_prod;

    sql = "SELECT cantidad FROM productos WHERE cod_prod = " + std::to_string(cod_prod);
    if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
        std::cerr << "Error al enviar la consulta al servidor de la base de datos." << std::endl;
        return;
    }

    memset(response, 0, sizeof(response));
    if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
        std::cerr << "Error al recibir la respuesta del servidor de la base de datos." << std::endl;
        return;
    }

    cantidad_disponible = atoi(response);
    if (cantidad_disponible == 0) {
        std::cout << "El producto seleccionado no está disponible." << std::endl;
        return;
    }

    int continuar = 1;
    while (continuar == 1) {
        std::cout << "¿Cuántas unidades quiere comprar? ";
        std::cin >> cantidad;
        while (cantidad < 0 || cantidad > cantidad_disponible) {
            std::cout << "Cantidad no válida. ¿Cuántas unidades quiere comprar? ";
            std::cin >> cantidad;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // Actualiza la cantidad del producto
        sql = "UPDATE productos SET cantidad = cantidad - " + std::to_string(cantidad) + " WHERE cod_prod = " + std::to_string(cod_prod);
        if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Error al enviar la consulta al servidor de la base de datos." << std::endl;
            return;
        }

        memset(response, 0, sizeof(response));
        if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
            std::cerr << "Error al recibir la respuesta del servidor de la base de datos." << std::endl;
            return;
        }

        std::cout << response;

        // Calcular el importe de la compra
        sql = "SELECT importe FROM productos WHERE cod_prod = " + std::to_string(cod_prod);
        if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Error al enviar la consulta al servidor de la base de datos." << std::endl;
            return;
        }

        memset(response, 0, sizeof(response));
        if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
            std::cerr << "Error al recibir la respuesta del servidor de la base de datos." << std::endl;
            return;
        }

        importe = atoi(response);

        // Realizar el pedido
        std::cout << "Introduzca su DNI: ";
        std::cin >> dni;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Introduzca su nombre: ";
        std::cin.getline(nom_clien, sizeof(nom_clien));

        sql = "INSERT INTO pedidos (dni, nom_clien, importe) VALUES ('" + std::string(dni) + "', '" + std::string(nom_clien) + "', " + std::to_string(importe * cantidad) + ")";
        if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Error al enviar la consulta al servidor de la base de datos." << std::endl;
            return;
        }

        memset(response, 0, sizeof(response));
        if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
            std::cerr << "Error al recibir la respuesta del servidor de la base de datos." << std::endl;
            return;
        }

        cod_ped = atoi(response);

        // Añadir el producto al pedido
        sql = "INSERT INTO productos_pedidos (cod_ped, cod_prod, cantidad) VALUES (" + std::to_string(cod_ped) + ", " + std::to_string(cod_prod) + ", " + std::to_string(cantidad) + ")";
        if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Error al enviar la consulta al servidor de la base de datos." << std::endl;
            return;
        }

        memset(response, 0, sizeof(response));
        if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
            std::cerr << "Error al recibir la respuesta del servidor de la base de datos." << std::endl;
            return;
        }

        std::cout << response;

        cantidad_total += cantidad;

        std::cout << "¿Desea comprar otro producto? (S/N): ";
        std::cin >> respuesta;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (respuesta != 'S' && respuesta != 's') {
            continuar = 0;
        }
    }

    // Actualizar el total pagado en el pedido
    sql = "UPDATE pedidos SET pagado = " + std::to_string(importe * cantidad_total) + " WHERE cod_ped = " + std::to_string(cod_ped);
    if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
        std::cerr << "Error al enviar la consulta al servidor de la base de datos." << std::endl;
        return;
    }

    memset(response, 0, sizeof(response));
    if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
        std::cerr << "Error al recibir la respuesta del servidor de la base de datos." << std::endl;
        return;
    }

    std::cout << response;

    // Cerrar la conexión con el servidor de la base de datos
    closesocket(socketFD);
}





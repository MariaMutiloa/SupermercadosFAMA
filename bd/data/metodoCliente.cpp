#include <iostream>
#include <cstring>
#include <unistd.h>
#include <winsock2.h>
#include "../sqlite3.h"
#include "metodoCliente.h"
#include "sockets.h"
#include <limits>
#include"../../src/cliente.h"
using namespace std;

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

    cout << "Ingrese su DNI: ";
    cin.getline(dni, 20);

    cout << "Ingrese su contraseña: ";
    cin.getline(contrasena, 50);

    // Enviar DNI y contraseña al servidor
    std::string data = std::string(dni) + " " + std::string(contrasena);
    if (send(socketFD, data.c_str(), data.length(), 0) == -1) {
        cerr << "Error al enviar los datos al servidor." << endl;
        return;
    }

    // Recibir respuesta del servidor
    char response[256];
    memset(response, 0, sizeof(response));
    if (recv(socketFD, response, sizeof(response), 0) == -1) {
        cerr << "Error al recibir la respuesta del servidor." << endl;
        return;
    }

    cout << response <<endl;

    // Verificar la existencia del DNI y contraseña en la base de datos
    if (strcmp(response, "OK") != 0) {
        cout << "El DNI o contraseña ingresados son incorrectos." << endl;
        return;
    }

    // Actualizar la información del cliente
    while (true) {
        cout << "¿Qué dato desea actualizar?" << endl;
        cout << "1. Contraseña" << endl;
        cout << "2. Dirección de casa" <<endl;
        cout << "3. Correo electrónico" <<endl;
        cout << "4. Salir" << endl;
        cout << "Ingrese el número de la opción que desee: ";
        cin.getline(opcion, 10);

        if (strcmp(opcion, "1") == 0) {
            char nuevaContrasena[50];
            char confirmacion[50];

            cout << "Ingrese su nueva contraseña: ";
            cin.getline(nuevaContrasena, 50);
            cout << "Confirme su nueva contraseña: ";
            cin.getline(confirmacion, 50);

            // Verifica que las contraseñas coincidan
            if (strcmp(nuevaContrasena, confirmacion) != 0) {
                cout << "Las contraseñas no coinciden, intente nuevamente." << endl;
                continue;
            }

            // Actualiza la contraseña
            sprintf(sql, "UPDATE cliente SET contrasena = '%s' WHERE dni = '%s'", nuevaContrasena, dni);
            if (send(socketFD, sql, strlen(sql), 0) == -1) {
                cerr << "Error al enviar la consulta al servidor." << endl;
                return;
            }

            memset(response, 0, sizeof(response));
            if (recv(socketFD, response, sizeof(response), 0) == -1) {
                cerr << "Error al recibir la respuesta del servidor." << endl;
                return;
            }

            cout << response << endl;

        } else if (strcmp(opcion, "2") == 0) {
            char nuevaDireccion[100];

            cout << "Ingrese su nueva dirección: ";
            cin.getline(nuevaDireccion, 100);

            // Actualiza la dirección en la base de datos
            sprintf(sql, "UPDATE cliente SET Direccion_Domicilio = '%s' WHERE dni = '%s'", nuevaDireccion, dni);
            if (send(socketFD, sql, strlen(sql), 0) == -1) {
                cerr << "Error al enviar la consulta al servidor." << endl;
                return;
            }

            memset(response, 0, sizeof(response));
            if (recv(socketFD, response, sizeof(response), 0) == -1) {
                cerr << "Error al recibir la respuesta del servidor." << endl;
                return;
            }

            cout << response << endl;

        } else if (strcmp(opcion, "3") == 0) {
            char nuevoCorreo[50];

            cout << "Ingrese su nuevo correo electrónico: ";
            cin.getline(nuevoCorreo, 50);

            // Actualiza el correo electrónico en la base de datos
            sprintf(sql, "UPDATE cliente SET Correo_electronico = '%s' WHERE dni = '%s'", nuevoCorreo, dni);
            if (send(socketFD, sql, strlen(sql), 0) == -1) {
                cerr << "Error al enviar la consulta al servidor." << endl;
                return;
            }

            memset(response, 0, sizeof(response));
            if (recv(socketFD, response, sizeof(response), 0) == -1) {
                cerr << "Error al recibir la respuesta del servidor." << endl;
                return;
            }

            cout << response << endl;

        } else if (strcmp(opcion, "4") == 0) {
            break;
        } else {
            cout << "Opción inválida, intente nuevamente." << endl;
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
        cerr << "Error al conectarse al servidor de la base de datos." << endl;
        return;
    }

    // Lista los productos disponibles
    cout << "Productos disponibles:" << endl;
    cout << "--------------------------------" << endl;
    std::string sql = "SELECT cod_prod, descripcion, importe, cantidad FROM productos";
    if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
        cerr << "Error al enviar la consulta al servidor de la base de datos." << endl;
        return;
    }

    char response[256];
    memset(response, 0, sizeof(response));
    if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
        cerr << "Error al recibir la respuesta del servidor de la base de datos." << endl;
        return;
    }

    cout << response;

    // Pedir al usuario que elija un producto y cantidad
    cout << "¿Qué producto quiere comprar? (introduzca el código): ";
    cin >> cod_prod;

    sql = "SELECT cantidad FROM productos WHERE cod_prod = " + std::to_string(cod_prod);
    if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
        cerr << "Error al enviar la consulta al servidor de la base de datos." <<endl;
        return;
    }

    memset(response, 0, sizeof(response));
    if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
        std::cerr << "Error al recibir la respuesta del servidor de la base de datos." << std::endl;
        return;
    }

    cantidad_disponible = atoi(response);
    if (cantidad_disponible == 0) {
        cout << "El producto seleccionado no está disponible." << endl;
        return;
    }

    int continuar = 1;
    while (continuar == 1) {
        cout << "¿Cuántas unidades quiere comprar? ";
        cin >> cantidad;
        while (cantidad < 0 || cantidad > cantidad_disponible) {
            cout << "Cantidad no válida. ¿Cuántas unidades quiere comprar? ";
            cin >> cantidad;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // Actualiza la cantidad del producto
        sql = "UPDATE productos SET cantidad = cantidad - " + std::to_string(cantidad) + " WHERE cod_prod = " + std::to_string(cod_prod);
        if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
            cerr << "Error al enviar la consulta al servidor de la base de datos." << endl;
            return;
        }

        memset(response, 0, sizeof(response));
        if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
            cerr << "Error al recibir la respuesta del servidor de la base de datos." << endl;
            return;
        }

        std::cout << response;

        // Calcular el importe de la compra
        sql = "SELECT importe FROM productos WHERE cod_prod = " + std::to_string(cod_prod);
        if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
            cerr << "Error al enviar la consulta al servidor de la base de datos." << endl;
            return;
        }

        memset(response, 0, sizeof(response));
        if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
            cerr << "Error al recibir la respuesta del servidor de la base de datos." << endl;
            return;
        }

        importe = atoi(response);

        // Realizar el pedido
        cout << "Introduzca su DNI: ";
        cin >> dni;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        cout << "Introduzca su nombre: ";
        cin.getline(nom_clien, sizeof(nom_clien));

        sql = "INSERT INTO pedidos (dni, nom_clien, importe) VALUES ('" + std::string(dni) + "', '" + std::string(nom_clien) + "', " + std::to_string(importe * cantidad) + ")";
        if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
            cerr << "Error al enviar la consulta al servidor de la base de datos." << endl;
            return;
        }

        memset(response, 0, sizeof(response));
        if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
            cerr << "Error al recibir la respuesta del servidor de la base de datos." << endl;
            return;
        }

        cod_ped = atoi(response);

        // Añadir el producto al pedido
        sql = "INSERT INTO productos_pedidos (cod_ped, cod_prod, cantidad) VALUES (" + std::to_string(cod_ped) + ", " + std::to_string(cod_prod) + ", " + std::to_string(cantidad) + ")";
        if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
            cerr << "Error al enviar la consulta al servidor de la base de datos." << endl;
            return;
        }

        memset(response, 0, sizeof(response));
        if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
            cerr << "Error al recibir la respuesta del servidor de la base de datos." <<endl;
            return;
        }

        cout << response;

        cantidad_total += cantidad;

        cout << "¿Desea comprar otro producto? (S/N): ";
        cin >> respuesta;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (respuesta != 'S' && respuesta != 's') {
            continuar = 0;
        }
    }

    // Actualizar el total pagado en el pedido
    sql = "UPDATE pedidos SET pagado = " + std::to_string(importe * cantidad_total) + " WHERE cod_ped = " + std::to_string(cod_ped);
    if (send(socketFD, sql.c_str(), sql.length(), 0) == SOCKET_ERROR) {
        cerr << "Error al enviar la consulta al servidor de la base de datos." << endl;
        return;
    }

    memset(response, 0, sizeof(response));
    if (recv(socketFD, response, sizeof(response), 0) == SOCKET_ERROR) {
        cerr << "Error al recibir la respuesta del servidor de la base de datos." << endl;
        return;
    }

    cout << response;

    // Cerrar la conexión con el servidor de la base de datos
    closesocket(socketFD);
}

//pasar el metodo a cpp con la creacion de sockets
void imprimirComprasCliente() {
    Cliente cliente;
    printf("DNI letra incluida: ");
    fgets(cliente.dni, sizeof(cliente.dni), stdin);
    sscanf(cliente.dni, "%s", cliente.dni);

    char *query = "SELECT c.Nombre, c.Apellido, p.Cod_ped, p.Importe, GROUP_CONCAT(pr.cod_prod || ' - ' || pr.descripcion, '; ') as Productos \
                   FROM cliente c, pedidoCliente p, prdidoCliente_productos pp, productos pr \
                   WHERE c.dni = c.dni AND p.dni = c.dni AND p.Cod_ped = pp.cod_ped AND pp.cod_prod = pr.cod_prod \
                   GROUP BY c.Nombre, c.Apellido, p.Cod_ped";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db ,query, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error en la preparación  del statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    printf(" ================\n");
    printf(" TUS COMPRAS:\n");
    printf(" ================\n\n");

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        char *nombre = (char *)sqlite3_column_text(stmt, 0);
        char *apellido = (char *)sqlite3_column_text(stmt, 1);
        int cod_ped = sqlite3_column_int(stmt, 2);
        int importe = sqlite3_column_int(stmt, 3);
        char *productos = (char *)sqlite3_column_text(stmt, 4);

        printf("Cliente: %s %s\n", nombre, apellido);
        printf("Codigo de pedido: %d\n", cod_ped);
        printf("Productos: %s\n", productos);
        printf("Importe: %d\n\n", importe);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error en la ejecucion de la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }
    sqlite3_finalize(stmt);
    // Cerrar la conexión con el servidor de la base de datos
    closesocket(socketFD); //da error porque queda crear el socket al principio
}




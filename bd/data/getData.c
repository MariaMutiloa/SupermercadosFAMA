#include "../sqLite/sqlite3.h"
#include "conexion.h"
#include <stdio.h>
#include "getData.h"
#include <string.h>
#include <stdlib.h>
#include "../src/struct.h" 

sqlite3 *db;

// Función para validar si el usuario es un cliente y si la contraseña es correcta
int validarCliente(char dni[], char contrasena[]) {
    // Preparar la consulta SQL
    char sql[200];
    snprintf(sql, sizeof(sql), "SELECT * FROM cliente WHERE dni='%s' AND contrasena='%s'", dni, contrasena);
    
    // Ejecutar la consulta y comprobar el resultado
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // El usuario existe y la contraseña es correcta
        return 1;
    } else {
        // El usuario no existe o la contraseña es incorrecta
        return 0;
    }
}

// Función para validar si el usuario es un administrador y si la contraseña es correcta
int validarAdministrador(char dni[], char contrasena[]) {
    // Preparar la consulta SQL
    char sql[200];
    snprintf(sql, sizeof(sql), "SELECT * FROM administrador WHERE dni='%s' AND contrasena='%s'", dni, contrasena);
    
    // Ejecutar la consulta y comprobar el resultado
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // El usuario existe y la contraseña es correcta
        return 1;
    } else {
        // El usuario no existe o la contraseña es incorrecta
        return 0;
    }
}


void agregarCliente() {
    
    Cliente cliente;

    printf("Ingrese los siguientes datos del cliente:\n");

    printf("DNI: ");
    fgets(cliente.dni, sizeof(cliente.dni), stdin);
    sscanf(cliente.dni, "%s", cliente.dni);

    printf("Nombre: ");
    fgets(cliente.nombre, sizeof(cliente.nombre), stdin);
    sscanf(cliente.nombre, "%s", cliente.nombre);

    printf("Apellido: ");
    fgets(cliente.apellido, sizeof(cliente.apellido), stdin);
    sscanf(cliente.apellido, "%s", cliente.apellido);

    printf("Direccion: ");
    fgets(cliente.direccion, sizeof(cliente.direccion), stdin);
    sscanf(cliente.direccion, "%s", cliente.direccion);

    printf("Correo electronico: ");
    fgets(cliente.correo, sizeof(cliente.correo), stdin);
    sscanf(cliente.correo, "%s", cliente.correo);

    printf("Numero de tarjeta: ");
    fgets(cliente.num_tarjeta, sizeof(cliente.num_tarjeta), stdin);
    sscanf(cliente.num_tarjeta, "%s", cliente.num_tarjeta);

    printf("Contrasena: ");
    fgets(cliente.contrasena, sizeof(cliente.contrasena), stdin);
    sscanf(cliente.contrasena, "%s", cliente.contrasena);

    if (!startConn()) {
        fprintf(stderr, "Error al conectar con la base de datos\n");
        return;
    }

    char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES (?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error en la preparacion del statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Asignar los valores de la estructura a los parametros de la consulta
    sqlite3_bind_text(stmt, 1, cliente.dni, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, cliente.nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, cliente.apellido, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, cliente.direccion, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, cliente.correo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, cliente.num_tarjeta, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, cliente.contrasena, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error en la ejecucion de la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    printf("Cliente agregado exitosamente a la base de datos\n");
}

void eliminarCliente() {
    sqlite3* db;
    char dni[10];
    char query[100];
    sqlite3_stmt* stmt;
    int result;

    // Conexion a la base de datos
    result = startConn(&db);

    if (!result) {
        fprintf(stderr, "Error conectando a la base de datos\n");
        return;
    }

    // Pedimos el DNI del cliente
    printf("Introduce el DNI del cliente que quiere eliminar: ");
    fgets(dni, 10, stdin);
    sscanf(dni, "%s", dni);

    // Creamos la consulta SQL para buscar al cliente
    sprintf(query, "SELECT * FROM cliente WHERE dni = '%s'", dni);

    // Compila la consulta SQL
    result = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    // Verifica si se pudo compilar la consulta SQL
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Ejecuta la consulta SQL
    result = sqlite3_step(stmt);

    // Verifica si se encontró al cliente
    if (result == SQLITE_ROW) {
        // Creamos la consulta SQL para eliminar al cliente
        sprintf(query, "DELETE FROM cliente WHERE dni = '%s'", dni);

        // Compila la consulta SQL
        result = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

        // Verifica si se pudo compilar la consulta SQL
        if (result != SQLITE_OK) {
            fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        // Ejecuta la consulta SQL
        result = sqlite3_step(stmt);

        // Verifica si se eliminó al cliente
        if (result != SQLITE_DONE) {
            fprintf(stderr, "Error eliminando al cliente: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        printf("Cliente eliminado correctamente\n");
    } else {
        printf("Cliente no encontrado\n");
    }

    // Libera la memoria del statement
    sqlite3_finalize(stmt);

    // Cierra la conexión a la base de datos
    sqlite3_close(db);
}

void imprimirCompras() {
    if (!startConn()) {
        fprintf(stderr, "Error al conectar con la base de datos\n");
        return;
    }

    char *query = "SELECT c.Nombre, c.Apellido, p.Cod_ped, p.Importe, GROUP_CONCAT(pr.cod_prod || ' - ' || pr.descripcion, '; ') as Productos \
                   FROM cliente c, pedidoCliente p, pedidoCliente_productos pp, productos pr \
                   WHERE c.dni = p.dni AND p.Cod_ped = pp.cod_ped AND pp.cod_prod = pr.cod_prod \
                   GROUP BY c.Nombre, c.Apellido, p.Cod_ped";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error en la preparacion del statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("Compras realizadas por los clientes:\n\n");

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
    sqlite3_close(db);
}


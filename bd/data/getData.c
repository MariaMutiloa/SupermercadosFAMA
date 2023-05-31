#include "../sqlite3.h"
#include "../conexion/conexion.h"
#include <stdio.h>
#include "getData.h"
#include <string.h>
#include <stdlib.h>
#include "../../src/struct.h"
sqlite3 *db;
// Mira si es usuario
int validarCliente(char dni[], char contrasena[])
{
    db = startConn();
    char sql[1000];
    snprintf(sql, sizeof(sql), "SELECT * FROM cliente WHERE dni='%s' AND contrasena='%s'", dni, contrasena);
    // ejecuta la consulta
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        // Es un usuario que existe y la contraseña es correcta
        sqlite3_finalize(stmt);
        int result = closeConn(db);
        if (result == SQLITE_OK)
        {
            printf("La conexión se ha cerrado correctamente.\n");
        }
        else
        {
            printf("Se ha producido un error al cerrar la conexión.\n");
        }
        return 1;
    }
    else
    {
        // El usuario no existe o la contraseña es incorrecta
        sqlite3_finalize(stmt);
        closeConn(db);
        return 0;
    }
}

// Mira si es administrador
int validarAdministrador(char dni[], char contrasena[])
{
    db = startConn();
    char sql[1000];
    snprintf(sql, sizeof(sql), "SELECT * FROM administrador WHERE dni='%s' AND contrasena='%s'", dni, contrasena);

    // Ejecutar la consulta
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        // El administrador existe y la contraseña es correcta
        sqlite3_finalize(stmt);
        int result = closeConn(db);
        if (result == SQLITE_OK)
        {
            printf("La conexión se ha cerrado correctamente.\n");
        }
        else
        {
            printf("Se ha producido un error al cerrar la conexión.\n");
        }
        return 1;
    }
    else
    {
        // El administrador no existe o la contraseña es incorrecta
        sqlite3_finalize(stmt);
        closeConn(db);
        return 0;
    }
}

// agrega un Cliente a la base de datos por teclado
void agregarCliente()
{
    Cliente cliente;
    db = startConn();
    if (db != NULL)
    {
        printf("Conexion con exito");
    }
    printf("Ingrese los siguientes datos del cliente:\n");

    printf("DNI letra incluida: ");
    fgets(cliente.dni, sizeof(cliente.dni), stdin);
    sscanf(cliente.dni, "%s", cliente.dni);

    getchar();

    printf("Nombre: ");
    fgets(cliente.nombre, sizeof(cliente.nombre), stdin);
    sscanf(cliente.nombre, "%s", cliente.nombre);

    printf("Apellido: ");
    fgets(cliente.apellido, sizeof(cliente.apellido), stdin);
    sscanf(cliente.apellido, "%s", cliente.apellido);

    printf("Direccion sin espacios: ");
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

    char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES (?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error en la preparacion del statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    // Le pasa los valores
    sqlite3_bind_text(stmt, 1, cliente.dni, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, cliente.nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, cliente.apellido, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, cliente.direccion, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, cliente.correo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, cliente.num_tarjeta, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, cliente.contrasena, -1, SQLITE_STATIC);
    // ejecuta la consulta
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Error en la ejecucion de la consulta: %s (%d)\n", sqlite3_errmsg(db), sqlite3_extended_errcode(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    sqlite3_finalize(stmt);
    int result = closeConn(db);
    if (result == SQLITE_OK)
    {
        printf("La conexión se ha cerrado correctamente.\n");
    }
    else
    {
        printf("Se ha producido un error al cerrar la conexión.\n");
    }

    printf("Cliente agregado exitosamente a la base de datos\n");
}
int calcularImporte(int cantidad, int cod_prod)
{
    int importe;
    sqlite3_stmt *stmt;

    // Saca el precio del producto
    if (sqlite3_prepare_v2(db, "SELECT importe FROM productos_proveedor WHERE cod_prod = ?", strlen("SELECT importe FROM productos_proveedor WHERE cod_prod = ?") + 1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    sqlite3_bind_int(stmt, 1, cod_prod);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        importe = cantidad * sqlite3_column_int(stmt, 0);
    }
    else
    {
        importe = 0;
    }
    sqlite3_finalize(stmt);
    printf("Importe calculado\n");

    return importe;
}
// eliminar cliente de la base de datos
void eliminarCliente()
{
    db = startConn();
    if (db == NULL)
    {
        fprintf(stderr, "Error conectando a la base de datos\n");
        return;
    }

    char dni[20];
    printf("Introduce el DNI del cliente que quiere eliminar: ");
    fgets(dni, sizeof(dni), stdin);
    dni[strcspn(dni, "\n")] = '\0';

    char *sql = "SELECT * FROM cliente WHERE dni = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_TRANSIENT);
    // ejecuta la consulta
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        sqlite3_finalize(stmt); // libera el puntero para el delete

        sql = "DELETE FROM cliente WHERE dni = ?";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
        sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_TRANSIENT);
        // ejecuta el delete
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
        {
            fprintf(stderr, "Error eliminando al cliente: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        printf("Cliente eliminado correctamente\n");
    }
    else
    {
        printf("Cliente no encontrado\n");
    }

    sqlite3_finalize(stmt);
    closeConn(db);
}

// imprime todas las compras que han realizado los clientes
void imprimirCompras()
{
    db = startConn();
    char *query = "SELECT c.Nombre, c.Apellido, p.Cod_ped, p.Importe, GROUP_CONCAT(pr.cod_prod || ' - ' || pr.descripcion, '; ') as Productos \
                   FROM cliente c, pedidoCliente p, prdidoCliente_productos pp, productos pr \
                   WHERE c.dni = p.dni AND p.Cod_ped = pp.cod_ped AND pp.cod_prod = pr.cod_prod \
                   GROUP BY c.Nombre, c.Apellido, p.Cod_ped";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error en la preparacion del statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("Compras realizadas por los clientes:\n\n");
    // ejecuta la consulta
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
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

    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Error en la ejecucion de la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    sqlite3_finalize(stmt);
    closeConn(db);
}

// realiza un pedido a los proveedores
void realizarPedido()
{
    db = startConn();
    int cod_prod, cantidad, cantidad_total = 0, cod_ped, importes = 0;
    char dni[10], nom_prov[50], respuesta;
    sqlite3_stmt *stmt;

    // Lista los productos que venden los proveedores
    printf("Lista de productos del proveedor:\n");
    printf("--------------------------------\n");
    if (sqlite3_prepare_v2(db, "SELECT cod_prod, descripcion, importe FROM productos_proveedor", strlen("SELECT cod_prod, descripcion, importe FROM productos_proveedor") + 1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("%d - %s: %d euros \n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_int(stmt, 2));
    }
    sqlite3_finalize(stmt);

    // Pide al usuario un producto y una cantidad
    int continuar = 1;
    while (continuar)
    {
        printf("¿Qué producto quiere pedir? (introduzca el código): ");
        scanf("%d", &cod_prod);
        fflush(stdin);
        printf("¿Cuántas unidades quiere pedir? ");
        scanf("%d", &cantidad);
        fflush(stdin);

        switch (sqlite3_prepare_v2(db, "UPDATE productos SET cantidad = cantidad + ? WHERE cod_prod IN (SELECT cod_prod FROM productos_proveedor WHERE cod_prod = ?)", strlen("UPDATE productos SET cantidad = cantidad + ? WHERE cod_prod IN (SELECT cod_prod FROM productos_proveedor WHERE cod_prod = ?)") + 1, &stmt, NULL))
        {
        case SQLITE_OK:
            sqlite3_bind_int(stmt, 1, cantidad);
            sqlite3_bind_int(stmt, 2, cod_prod);
            // ejecuta la actualizacion
            if (sqlite3_step(stmt) == SQLITE_DONE)
            {
                cantidad_total += cantidad;
                printf("¿Quiere pedir otro producto? (s/n) ");
                scanf(" %c", &respuesta);
                if (respuesta == 'n')
                {
                    continuar = 0;
                }
            }
            else
            {
                printf("Error en la actualización de la cantidad de productos\n");
                fprintf(stderr, "Error updating data: %s\n", sqlite3_errmsg(db));
                continuar = 0;
            }
            sqlite3_finalize(stmt);
            break;
        default:
            printf("Error en la preparación de la consulta\n");
            fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
            continuar = 0;
            break;
        }
    }

    // Genera un código de pedido
    switch (sqlite3_prepare_v2(db, "SELECT MAX(cod_ped) FROM pedidoAdministrador", -1, &stmt, NULL))
    {
    case SQLITE_OK:
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            cod_ped = sqlite3_column_int(stmt, 0) + 1;
        }
        else
        {
            cod_ped = 1;
        }
        sqlite3_finalize(stmt);
        break;
    default:
        printf("Error en la preparación de la consulta\n");
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("Introduzca su DNI: ");
    scanf("%s", dni);
    fflush(stdin);

    switch (sqlite3_prepare_v2(db, "INSERT INTO pedidoAdministrador(cod_ped, dni,importe, pagado) VALUES(?, ?, ?, 0)", strlen("INSERT INTO pedidoAdministrador(cod_ped, dni, importe, pagado) VALUES(?, ?, ?, 0)") + 1, &stmt, NULL))
    {
    // al principio siempre pagado será 0 ya que se paga a 3 meses.
    case SQLITE_OK:
        sqlite3_bind_int(stmt, 1, cod_ped);
        sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_TRANSIENT);
        importes = calcularImporte(cantidad, cod_prod);
        sqlite3_bind_int(stmt, 3, importes);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            printf("El pedido se ha registrado correctamente\n");
        }
        else
        {
            printf("Error al registrar el pedido\n");
            fprintf(stderr, "Error updating data: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
        break;
    default:
        printf("Error en la preparación de la consulta\n");
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Aumenta la cantidad de productos en almacen
    switch (sqlite3_prepare_v2(db, "UPDATE productos SET cantidad = cantidad + ? WHERE cod_prod = ?", strlen("UPDATE productos SET cantidad = cantidad + ? WHERE cod_prod = ?") + 1, &stmt, NULL))
    {
    case SQLITE_OK:
        sqlite3_bind_int(stmt, 1, cantidad);
        sqlite3_bind_int(stmt, 2, cod_prod);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            printf("Inventario actualizado correctamente\n");
        }
        else
        {
            printf("Error al actualizar el inventario\n");
            fprintf(stderr, "Error updating data: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
        break;
    default:
        printf("Error en la preparación de la consulta\n");
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_finalize(stmt);
    closeConn(db);
}

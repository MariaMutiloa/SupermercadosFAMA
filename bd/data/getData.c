#include "../sqlite3.h"
#include "../conexion/conexion.h"
#include <stdio.h>
#include "getData.h"
#include <string.h>
#include <stdlib.h>
#include "../../src/struct.h" 

sqlite3 *db;

// Función para validar si el usuario es un cliente y si la contraseña es correcta
int validarCliente(char dni[], char contrasena[]) {
    usleep(10000);
    db = startConn();
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
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
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
    closeConn(db);
}

// Función para validar si el usuario es un administrador y si la contraseña es correcta
int validarAdministrador(char dni[], char contrasena[]) {
    usleep(10000);
    db=startConn();
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
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
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
    closeConn(db);
}


void agregarCliente() {
    
    Cliente cliente;
      usleep(10000);
    db = startConn();
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
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
        fprintf(stderr, "Error en la ejecucion de la consulta: %s (%d)\n", sqlite3_errmsg(db), sqlite3_extended_errcode(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
    closeConn(db);

    printf("Cliente agregado exitosamente a la base de datos\n");
}
int calcularImporte(int cantidad, int cod_prod) {
    int importe;
    sqlite3_stmt *stmt;

    // Obtiene el precio del producto de la tabla productos_proveedor
    if(sqlite3_prepare_v2(db, "SELECT importe FROM productos_proveedor WHERE cod_prod = ?", strlen("SELECT importe FROM productos_proveedor WHERE cod_prod = ?")+1, &stmt, NULL) != SQLITE_OK){
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    sqlite3_bind_int(stmt, 1, cod_prod);
    if(sqlite3_step(stmt) == SQLITE_ROW){
        importe = cantidad * sqlite3_column_int(stmt, 0);
    } else {
        importe = 0;
    }
    sqlite3_finalize(stmt);
    printf("Importe calculado\n");

    return importe;
}

void eliminarCliente() {
    usleep(10000);
    db = startConn();
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (db == NULL) {
        fprintf(stderr, "Error conectando a la base de datos\n");
        return;
    }

    char dni[20];
    printf("Introduce el DNI del cliente que quiere eliminar: ");
    fgets(dni, sizeof(dni), stdin);
    dni[strcspn(dni, "\n")] = '\0';

    char* sql = "SELECT * FROM cliente WHERE dni = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        sqlite3_finalize(stmt);

        sql = "DELETE FROM cliente WHERE dni = ?";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_TRANSIENT);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error eliminando al cliente: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        printf("Cliente eliminado correctamente\n");
    } else {
        printf("Cliente no encontrado\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
    closeConn(db);
}



void imprimirCompras() {
    usleep(10000); // pausa de 10 milisegundos
    db=startConn();
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    char *query = "SELECT c.Nombre, c.Apellido, p.Cod_ped, p.Importe, GROUP_CONCAT(pr.cod_prod || ' - ' || pr.descripcion, '; ') as Productos \
                   FROM cliente c, pedidoCliente p, prdidoCliente_productos pp, productos pr \
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
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
    closeConn(db);
}

void realizarPedido(){
    
    usleep(10000); // pausa de 10 milisegundos
    db=startConn();
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    int cod_prod, cantidad, cantidad_total = 0, cod_ped, importes = 0;
    char dni[10], nom_prov[50], respuesta;
    sqlite3_stmt *stmt;

    // Muestra la lista de productos del proveedor
    printf("Lista de productos del proveedor:\n");
    printf("--------------------------------\n");
    if(sqlite3_prepare_v2(db, "SELECT cod_prod, descripcion, importe FROM productos_proveedor", strlen("SELECT cod_prod, descripcion, importe FROM productos_proveedor")+1, &stmt, NULL) != SQLITE_OK){
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    while(sqlite3_step(stmt) != SQLITE_DONE){
        printf("%d - %s: %d euros \n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_int(stmt, 2));
    }
    sqlite3_finalize(stmt);

    // Pedir al usuario que elija un producto y la cantidad
    int continuar = 1;
    while(continuar){
        printf("¿Qué producto quiere pedir? (introduzca el código): ");
        scanf("%d", &cod_prod);
        fflush(stdin);
        printf("¿Cuántas unidades quiere pedir? ");
        scanf("%d", &cantidad);
        fflush(stdin);

        // Actualiza la cantidad del producto en la tabla productos
        switch(sqlite3_prepare_v2(db, "UPDATE productos SET cantidad = cantidad + ? WHERE cod_prod IN (SELECT cod_prod FROM productos_proveedor WHERE cod_prod = ?)",strlen("UPDATE productos SET cantidad = cantidad + ? WHERE cod_prod IN (SELECT cod_prod FROM productos_proveedor WHERE cod_prod = ?)")+1, &stmt, NULL)){
            case SQLITE_OK:
                sqlite3_bind_int(stmt, 1, cantidad);
                sqlite3_bind_int(stmt, 2, cod_prod);

                if(sqlite3_step(stmt) == SQLITE_DONE){
                    cantidad_total += cantidad;
                    printf("¿Quiere pedir otro producto? (s/n) ");
                    scanf(" %c", &respuesta);
                    if(respuesta == 'n'){
                        continuar = 0;
                    }
                } else {
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

    // Genera un código de pedido único
    switch(sqlite3_prepare_v2(db, "SELECT MAX(cod_ped) FROM pedidoAdministrador", -1, &stmt, NULL)){
        case SQLITE_OK:
            if(sqlite3_step(stmt) == SQLITE_ROW){
                cod_ped = sqlite3_column_int(stmt, 0) + 1;
            } else {
                cod_ped = 1;
            }
            sqlite3_finalize(stmt);
            break;
        default:
            printf("Error en la preparación de la consulta\n");
            fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
            return;
    }

    // Pedir el DNI del administrador
printf("Introduzca su DNI: ");
scanf("%s", dni);
fflush(stdin);


// Genera un registro de pedido único
switch(sqlite3_prepare_v2(db, "INSERT INTO pedidoAdministrador(cod_ped, dni,importe, pagado) VALUES(?, ?, ?, 0)", strlen("INSERT INTO pedidoAdministrador(cod_ped, dni, importe, pagado) VALUES(?, ?, ?, 0)")+1, &stmt, NULL)){
    //al principio siempre pagado será 0 ya que se paga a 3 meses.
    case SQLITE_OK:
        sqlite3_bind_int(stmt, 1, cod_ped);
        sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_TRANSIENT);
        importes = calcularImporte(cantidad,cod_prod);
        sqlite3_bind_int(stmt, 3, importes);

        if(sqlite3_step(stmt) == SQLITE_DONE){
            printf("El pedido se ha registrado correctamente\n");
        } else {
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

// Actualiza la tabla de inventario para disminuir la cantidad de los productos pedidos
switch(sqlite3_prepare_v2(db, "UPDATE productos SET cantidad = cantidad + ? WHERE cod_prod = ?", strlen("UPDATE productos SET cantidad = cantidad + ? WHERE cod_prod = ?")+1, &stmt, NULL)){
    case SQLITE_OK:
        sqlite3_bind_int(stmt, 1, cantidad);
        sqlite3_bind_int(stmt, 2, cod_prod);

        if(sqlite3_step(stmt) == SQLITE_DONE){
            printf("Inventario actualizado correctamente\n");
        } else {
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
sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
closeConn(db);
}

void actualizarDatosCliente() {
    char dni[10];
    char contrasena[50];
    char opcion[10];
    char sql[200];
    sqlite3_stmt *stmt;
    int rc;
    usleep(10000); // pausa de 10 milisegundos
 
    db=startConn();
    // Ingresar el DNI del cliente
    printf("Ingrese su DNI: ");
    fgets(dni, 10, stdin);
    dni[strcspn(dni, "\r\n")] = 0;
    getchar();

    // Ingresar la contraseña del cliente
    startConn();
    printf("Ingrese su contraseña: ");
    fgets(contrasena, 50, stdin);
    contrasena[strcspn(contrasena, "\r\n")] = 0;

    // Inicia un bloque de control de transacciones
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    if (rc != SQLITE_OK) {
        printf("Error al iniciar la transacción: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Actualizar la información del cliente
    while (1) {
        // Mostrar opciones al usuario
        printf("\n¿Qué dato desea actualizar?\n");
        printf("1. Contraseña\n");
        printf("2. Dirección de casa\n");
        printf("3. Correo electrónico\n");
        printf("4. Salir\n");
        printf("Ingrese el número de la opción que desee: ");

        // Lee la opción del usuario
        fgets(opcion, 10, stdin);
        opcion[strcspn(opcion, "\r\n")] = 0;

        if (strcmp(opcion, "1") == 0) {
            // Pedir la nueva contraseña al usuario
            char nuevaContrasena[50];
            char confirmacion[50];

            printf("\nIngrese su nueva contraseña: ");
            fgets(nuevaContrasena, 50, stdin);
            nuevaContrasena[strcspn(nuevaContrasena, "\r\n")] = 0;
            // Pide la confirmación de la nueva contraseña al usuario
            printf("Confirme su nueva contraseña: ");
            fgets(confirmacion, 50, stdin);
            confirmacion[strcspn(confirmacion, "\r\n")] = 0;

            // Verifica que las contraseñas coincidan
            if (strcmp(nuevaContrasena, confirmacion) != 0) {
                printf("Las contraseñas no coinciden. Por favor, inténtelo nuevamente.\n");
            } else {
                 
  sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
                // Actualiza la contraseña del cliente en la base de datos
                snprintf(sql, sizeof(sql), "UPDATE cliente SET contrasena='%s' WHERE dni='%s' AND contrasena='%s'", nuevaContrasena, dni, contrasena);
                rc = sqlite3_exec(db, sql, 0, 0, 0);

                if (rc != SQLITE_OK) {
printf("Error al actualizar la contraseña del cliente: %s\n", sqlite3_errmsg(db));
} else {
printf("Contraseña actualizada exitosamente.\n");
strcpy(contrasena, nuevaContrasena); // Actualiza la variable de la contraseña con la nueva contraseña
}
} }else if (strcmp(opcion, "2") == 0) {
// Pedir la nueva dirección al usuario
char nuevaDireccion[100];
        printf("\nIngrese su nueva dirección: ");
        fgets(nuevaDireccion, 100, stdin);
        nuevaDireccion[strcspn(nuevaDireccion, "\r\n")] = 0;

        // Actualiza la dirección del cliente en la base de datos
        snprintf(sql, sizeof(sql), "UPDATE cliente SET direccion='%s' WHERE dni='%s' AND contrasena='%s'", nuevaDireccion, dni, contrasena);
        rc = sqlite3_exec(db, sql, 0, 0, 0);

        if (rc != SQLITE_OK) {
            printf("Error al actualizar la dirección del cliente: %s\n", sqlite3_errmsg(db));
        } else {
            printf("Dirección actualizada exitosamente.\n");
        }
    } else if (strcmp(opcion, "3") == 0) {
        // Pedir el nuevo correo electrónico al usuario
        char nuevoCorreo[50];

        printf("\nIngrese su nuevo correo electrónico: ");
        fgets(nuevoCorreo, 50, stdin);
        nuevoCorreo[strcspn(nuevoCorreo, "\r\n")] = 0;

        // Actualiza el correo electrónico del cliente en la base de datos
        snprintf(sql, sizeof(sql), "UPDATE cliente SET correo='%s' WHERE dni='%s' AND contrasena='%s'", nuevoCorreo, dni, contrasena);
        rc = sqlite3_exec(db, sql, 0, 0, 0);

        if (rc != SQLITE_OK) {
            printf("Error al actualizar el correo electrónico del cliente: %s\n", sqlite3_errmsg(db));
        } else {
            printf("Correo electrónico actualizado exitosamente.\n");
        }
    } else if (strcmp(opcion, "4") == 0) {
        // Sale del bucle
        break;
    } else {
        // Opción inválida
        printf("Opción inválida. Por favor, inténtelo nuevamente.\n");
    }
}

// Cierra el bloque de control de transacciones
rc = sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);

if (rc != SQLITE_OK) {
    printf("Error al cerrar la transacción: %s\n", sqlite3_errmsg(db));
}

// Cierra la conexión con la base de datos
sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
sqlite3_close(db);
}

 void realizarCompra(){
        
        usleep(10000); // pausa de 10 milisegundos
        db=startConn();
        sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
        char respuesta;
        char dni[20];
        int cod_ped, importe, pagado, cantidad_total = 0;
        int cod_prod, cantidad;
        char descripcion[100], nom_clien[100];
        sqlite3_stmt *stmt;
        int cantidad_disponible;
      //  int importes;

        // Muestra la lista de productos del proveedor
        printf("Productos disponibles:\n");
        printf("--------------------------------\n");
        if(sqlite3_prepare_v2(db, "SELECT cod_prod, descripcion, importe FROM productos", strlen("SELECT cod_prod, descripcion, importe FROM productos")+1, &stmt, NULL) != SQLITE_OK){
            fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
            return;
        }
        while(sqlite3_step(stmt) != SQLITE_DONE){
            printf("%d - %s: %d euros %d unidades \n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_int(stmt, 2), sqlite3_column_int(stmt, 3), sqlite3_column_int(stmt, 4));
        }
        sqlite3_finalize(stmt);

        // Pedir al usuario que elija un producto y la cantidad
       
            printf("¿Qué producto quiere comprar? (introduzca el código): ");
            scanf("%d", &cod_prod);

             if(sqlite3_prepare_v2(db, "SELECT cantidad FROM productos WHERE cod_prod = ?", -1, &stmt, NULL) != SQLITE_OK){
             fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
            return;
        }

            sqlite3_bind_int(stmt, 1, cod_prod);
            if(sqlite3_step(stmt) == SQLITE_ROW){
                cantidad_disponible = sqlite3_column_int(stmt, 0);
            
        }
            sqlite3_finalize(stmt);


    int continuar = 1;
    while(continuar){
            printf("¿Cuántas unidades quiere comprar? ");
            scanf("%d", cantidad);
            while(cantidad < 0 || cantidad > cantidad_disponible){
                printf("¿Cuántas unidades quiere comprar? ");
                scanf("%d", &cantidad);
                fflush(stdin);
            }

            // Actualiza la cantidad del producto en la tabla productos
            switch(sqlite3_prepare_v2(db, "UPDATE productos SET cantidad = cantidad - ? WHERE cod_prod IN (SELECT cod_prod FROM productos WHERE cod_prod = ?)",strlen("UPDATE productos SET cantidad = cantidad - ? WHERE cod_prod IN (SELECT cod_prod FROM productos WHERE cod_prod = ?)")+1, &stmt, NULL)){
                case SQLITE_OK:
                    sqlite3_bind_int(stmt, 1, cantidad);
                    sqlite3_bind_int(stmt, 2, cod_prod);

                    if(sqlite3_step(stmt) == SQLITE_DONE){
                        cantidad_total -= cantidad;
                        printf("¿Quiere pedir otro comprar? (s/n) ");
                        scanf(" %c", &respuesta);
                        if(respuesta == 'n'){
                            continuar = 0;
                        }
                    } else {
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


        // Genera un código de pedido único
        switch(sqlite3_prepare_v2(db, "SELECT MAX(cod_ped) FROM pedidoCliente", -1, &stmt, NULL)){
            case SQLITE_OK:
                if(sqlite3_step(stmt) == SQLITE_ROW){
                    cod_ped = sqlite3_column_int(stmt, 0) + 1;
                } else {
                    cod_ped = 1;
                }
                sqlite3_finalize(stmt);
                break;
            default:
                printf("Error en la preparación de la consulta\n");
                fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
                return;
        }

      // Pedir el DNI del comprador
        printf("Introduzca su DNI: ");
        scanf("%s", dni);
        fflush(stdin);


    // Genera un registro de pedido único
    switch(sqlite3_prepare_v2(db, "INSERT INTO pedidoCliente(cod_ped, dni,importe, pagado) VALUES(?, ?, ?, 0)", strlen("INSERT INTO pedidoCliente(cod_ped, dni, importe, pagado) VALUES(?, ?, ?, 0)")+1, &stmt, NULL)){
        //al principio siempre pagado será 0 ya que se paga a 3 meses.
        case SQLITE_OK:
            sqlite3_bind_int(stmt, 1, cod_ped);
            sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_TRANSIENT);
            int importes = calcularImporte(cantidad,cod_prod);
            sqlite3_bind_int(stmt, 3, importes);

            if(sqlite3_step(stmt) == SQLITE_DONE){
                printf("La comrpa se ha registrado correctamente\n");
            } else {
                printf("Error al registrar el compra\n");
                fprintf(stderr, "Error updating data: %s\n", sqlite3_errmsg(db));
            }
            sqlite3_finalize(stmt);
            break;
        default:
            printf("Error en la preparación de la consulta\n");
            fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
            return;
    }


    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
    closeConn(db);
    }
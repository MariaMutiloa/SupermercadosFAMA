#include "../sqlite3.h"
#include "../conexion/conexion.h"
#include <stdio.h>
#include "getData.h"
#include <string.h>
#include <stdlib.h>
#include "../../src/struct.h" 

sqlite3 *db;

// Función para ver si el usuario es un cliente y si la contraseña es correcta
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

// Función para ver si el usuario es un administrador y si la contraseña es correcta
int validarAdministrador(char dni[], char contrasena[]) {
    // Prepara la consulta SQL
    char sql[200];
    snprintf(sql, sizeof(sql), "SELECT * FROM administrador WHERE dni='%s' AND contrasena='%s'", dni, contrasena);
    
    // Ejecuta la consulta y comprobar el resultado
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

    // Asigna los valores de la estructura a los parametros de la consulta
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
    sqlite3_close(db);

    printf("Cliente agregado exitosamente a la base de datos\n");
}
int calcularImporte(int cantidad, int cod_prod) {
    int importe;
    sqlite3_stmt *stmt;

    // Obtiene el precio del producto de la tabla productos_proveedor
    if(sqlite3_prepare_v2(db, "SELECT importe FROM productos_proveedor WHERE cod_prod = ?", -1, &stmt, NULL) != SQLITE_OK){
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

    return importe;
}

void eliminarCliente() {
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

    // Pide el DNI del cliente
    printf("Introduce el DNI del cliente que quiere eliminar: ");
    fgets(dni, 10, stdin);
    sscanf(dni, "%s", dni);

    // Crea la consulta SQL para buscar al cliente
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
    sqlite3_close(db);
}

void listaProductosProveedor(){
    sqlite3_close(db);
    startConn();
    int cod_ped, importe, pagado;
    int cod_prod;
    char descripcion[100];
    sqlite3_stmt *stmt;

    printf("Lista de productos del proveedor:\n");
    printf("--------------------------------\n");
    // Muestra los productos del proveedor
    if(sqlite3_prepare_v2(db, "SELECT cod_prod, descripcion, importe FROM productos_proveedor", strlen("SELECT cod_prod, descripcion, importe FROM productos_proveedor")+1, &stmt, NULL) != SQLITE_OK){
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    while(sqlite3_step(stmt) != SQLITE_DONE){
        printf("%d - %s: %d euros \n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_int(stmt, 2), sqlite3_column_int(stmt, 3));
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

    void realizarPedido(){
    startConn();
    char respuesta;
    char dni[20];
    int cod_ped, importe, pagado, cantidad_total = 0;
    int cod_prod, cantidad;
    char descripcion[100], nom_prov[100];
    sqlite3_stmt *stmt;

    // Pedir al usuario que elija un producto y la cantidad
    do{
        printf("¿Qué producto quiere pedir? (introduzca el código): ");
        scanf("%d", &cod_prod);
        fflush(stdin);
        printf("¿Cuántas unidades quiere pedir? ");
        scanf("%d", &cantidad);
        fflush(stdin);
        // Actualiza la cantidad del producto en la tabla productos
        if(sqlite3_prepare_v2(db, "UPDATE productos SET cantidad = cantidad + ? WHERE cod_prod = ?",strlen("UPDATE productos SET cantidad = cantidad + ? WHERE cod_prod = ?")+1, &stmt, NULL) != SQLITE_OK){
            fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
            return;
        }
        sqlite3_bind_int(stmt, 1, cantidad);
        sqlite3_bind_int(stmt, 2, cod_prod);

        if(sqlite3_step(stmt) != SQLITE_DONE){
            printf("Error en linea 309\n");
            fprintf(stderr, "Error updating data: %s\n", sqlite3_errmsg(db));
            return;
        }
        sqlite3_finalize(stmt);

        cantidad_total += cantidad;

        printf("¿Quiere pedir otro producto? (s/n) ");
        scanf(" %c", &respuesta);
    } while(respuesta == 's');

    // Genera un código de pedido único
    if(sqlite3_prepare_v2(db, "SELECT MAX(cod_ped) FROM pedidoAdministrador", -1, &stmt, NULL) != SQLITE_OK){
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    if(sqlite3_step(stmt) == SQLITE_ROW){
        cod_ped = sqlite3_column_int(stmt, 0) + 1;
    } else {
        cod_ped = 1;
    }
    sqlite3_finalize(stmt);

    // Pedir el DNI del administrador
    printf("Introduzca su DNI: ");
    scanf("%s", dni);

    importe = calcularImporte(cantidad, cod_prod);

// Inserta el pedido en la tabla pedidoAdministrador
    pagado = (respuesta == 's') ? 1 : 0; // Si respuesta es 's', pagado es 1, sino es 0
    if(sqlite3_prepare_v2(db, "INSERT INTO pedidoAdministrador (dni, cod_ped, importe, pagado) VALUES (?, ?, ?, ?)", -1, &stmt, NULL) != SQLITE_OK){
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, cod_ped);
    sqlite3_bind_double(stmt, 3, importe);
    sqlite3_bind_int(stmt, 4, pagado);


// Obtiene la descripción del producto y el nombre del proveedor
if(sqlite3_prepare_v2(db, "SELECT descripcion, nom_prov FROM productos_proveedor WHERE cod_prod = ?", -1, &stmt, NULL) != SQLITE_OK){
    fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
    return;
}
sqlite3_bind_int(stmt, 1, cod_prod);
if(sqlite3_step(stmt) == SQLITE_ROW){
    strcpy(descripcion, sqlite3_column_text(stmt, 0));
    strcpy(nom_prov, sqlite3_column_text(stmt, 1));
}
sqlite3_finalize(stmt);

// Inserta los detalles del pedido en la tabla detallesPedido
if(sqlite3_prepare_v2(db, "INSERT INTO detallesPedido (cod_ped, cod_prod, descripcion, cantidad, importe, nom_prov) VALUES (?, ?, ?, ?, ?, ?)", -1, &stmt, NULL) != SQLITE_OK){
    fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
    return;
}
sqlite3_bind_int(stmt, 1, cod_ped);
sqlite3_bind_int(stmt, 2, cod_prod);
sqlite3_bind_text(stmt, 3, descripcion, -1, SQLITE_TRANSIENT);
sqlite3_bind_int(stmt, 4, cantidad_total);
sqlite3_bind_int(stmt, 5, importe);
sqlite3_bind_text(stmt, 6, nom_prov, -1, SQLITE_TRANSIENT);
if(sqlite3_step(stmt) != SQLITE_DONE){
    fprintf(stderr, "Error inserting data: %s\n", sqlite3_errmsg(db));
    return;
}
sqlite3_finalize(stmt);
sqlite3_close(db);

printf("Pedido realizado con éxito\n");
}

void actualizarDatosCliente() {
    char dni[10];
    char contrasena[50];
    char opcion[10];
    char sql[200];
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    
    // Realiza la conexión con la base de datos
    rc = sqlite3_open("./fama.db", &db);
    if (rc != SQLITE_OK) {
        printf("Error al conectar con la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    
   // Ingresa el DNI del cliente
printf("Ingrese su DNI: ");
fgets(dni, 10, stdin);
dni[strcspn(dni, "\r\n")] = 0;

getchar();

    // Verifica si el DNI del cliente existe en la base de datos
    snprintf(sql, sizeof(sql), "SELECT * FROM cliente WHERE dni='%s'", dni);
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al verificar el DNI del cliente: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        printf("El DNI ingresado no existe en la base de datos.\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }
    
    // Ingresa la contraseña del cliente
printf("Ingrese su contraseña: ");
fgets(contrasena, 50, stdin);
contrasena[strcspn(contrasena, "\r\n")] = 0;
    
    // Verifica la contraseña del cliente
    snprintf(sql, sizeof(sql), "SELECT * FROM cliente WHERE dni='%s' AND contrasena='%s'", dni, contrasena);
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al verificar la contraseña del cliente: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        printf("Contraseña incorrecta. Por favor, inténtelo nuevamente.\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Bucle para preguntar al usuario qué datos quiere cambiar
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

        // opción del usuario
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
            // Actualiza la contraseña del cliente en la base de datos
            snprintf(sql, sizeof(sql), "UPDATE cliente SET contrasena='%s' WHERE dni='%s'", nuevaContrasena, dni);
            rc = sqlite3_exec(db, sql, 0, 0, 0);
                        if (rc != SQLITE_OK) {
                printf("Error al actualizar la contraseña: %s\n", sqlite3_errmsg(db));
            } else {
                printf("Contraseña actualizada exitosamente.\n");
            }
        }
    } else if (strcmp(opcion, "2") == 0) {
        // Pide la nueva dirección de casa al usuario
        char nuevaDireccion[200];

        printf("\nIngrese su nueva dirección de casa: ");
        fgets(nuevaDireccion, 200, stdin);
        nuevaDireccion[strcspn(nuevaDireccion, "\r\n")] = 0;

        // Actualiza la dirección de casa del cliente 
        snprintf(sql, sizeof(sql), "UPDATE cliente SET direccion='%s' WHERE dni='%s'", nuevaDireccion, dni);
        rc = sqlite3_exec(db, sql, 0, 0, 0);

        if (rc != SQLITE_OK) {
            printf("Error al actualizar la dirección de casa: %s\n", sqlite3_errmsg(db));
        } else {
            printf("Dirección de casa actualizada exitosamente.\n");
        }
    } else if (strcmp(opcion, "3") == 0) {
        // Pide el nuevo correo electrónico al usuario
        char nuevoCorreo[50];

        printf("\nIngrese su nuevo correo electrónico: ");
        fgets(nuevoCorreo, 50, stdin);
        nuevoCorreo[strcspn(nuevoCorreo, "\r\n")] = 0;

        // Actualiza el correo electrónico del cliente 
        snprintf(sql, sizeof(sql), "UPDATE cliente SET correo='%s' WHERE dni='%s'", nuevoCorreo, dni);
        rc = sqlite3_exec(db, sql, 0, 0, 0);

        if (rc != SQLITE_OK) {
            printf("Error al actualizar el correo electrónico: %s\n", sqlite3_errmsg(db));
        } else {
            printf("Correo electrónico actualizado exitosamente.\n");
        }
    } else if (strcmp(opcion, "4") == 0) {
        // Salir del bucle y de la función
        break;
    } else {
        printf("Opción inválida. Por favor, seleccione una opción válida.\n");
    }
}

// Cerrar la conexión con la base de datos
sqlite3_finalize(stmt);
sqlite3_close(db);
}
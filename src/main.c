#include "../sqLite/sqlite3.h"
#include "../bd/conexion/conexion.h"
#include "../bd/data/getData.h"
#include "menuAdministrador.h"
#include <stdio.h>
#include <stdlib.h>


sqlite3 *db;

int main(){
     startConn();

    // Insertar 20 productos básicos en la tabla "productos"
    char *productos[20][3] = {
        {"1.5", "1", "Leche"},
        {"2", "2", "Pan"},
        {"2.5", "3", "Huevos"},
        {"1.7", "4", "Arroz"},
        {"3.4", "5", "Berenjena"},
        {"4.5", "6", "Pollo"},
        {"7.9", "7", "Merluza"},
        {"8", "8", "Picadillo de ternera"},
        {"2.3", "9", "Cebolla"},
        {"3.5", "10", "Tomate"},
        {"5.6", "11", "Patata"},
        {"1.25", "12", "Zanahoria"},
        {"2", "13", "Lechuga"},
        {"3", "14", "Pimiento"},
        {"1", "15", "Ajo"},
        {"2", "16", "Plátano"},
        {"2", "17", "Manzana"},
        {"3", "18", "Naranja"},
        {"4", "19", "Melón"},
        {"3", "20", "Sandía"}
    };

    for(int i = 0; i < 20; i++){
        char *importe = productos[i][0];
        char *cod_prod = productos[i][1];
        char *descripcion = productos[i][2];

        char sql[128];
        sprintf(sql, "INSERT INTO productos (importe, cod_prod, descripcion) VALUES (%s, %s, '%s');", importe, cod_prod, descripcion);

        int rc = sqlite3_exec(db, sql, 0, 0, 0);
        if(rc != SQLITE_OK){
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            return 1;
        }
    }

    sqlite3_close(db);

    startConn();
    
    char dni[20], contrasena[20];
    int opcion;
    
    printf("Bienvenido!\n");
    printf("Por favor, ingrese su DNI: ");
    fgets(dni, sizeof(dni), stdin);
    dni[strcspn(dni, "\n")] = 0; // Eliminar el salto de línea final
    
    printf("Por favor, ingrese su contraseña: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = 0; // Eliminar el salto de línea final
    
    if (validarCliente(dni, contrasena)) {
        printf("Bienvenido, cliente!\n");
        // Mostrar menú de cliente
       
    } else if (validarAdministrador(dni, contrasena)) {
        printf("Bienvenido, administrador!\n");
        admin_menu;
    
    } else {
        printf("Usuario o contraseña incorrectos.\n");
    }
    
    sqlite3_close(db);
    return 0;
}

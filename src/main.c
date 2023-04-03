#include "../bd/sqlite3.h"
#include "../bd/conexion/conexion.h"
#include "../bd/data/getData.h"
#include "menuAdministrador.h"
#include "menuCliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sqlite3 *db;

int main(){

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

       
    } else if (validarAdministrador(dni, contrasena)) {
        printf("Bienvenido, administrador!\n");
        admin_menu();
    
    } else {
        printf("Usuario o contraseña incorrectos.\n");
    }
    
    sqlite3_close(db);
    return 0;
}

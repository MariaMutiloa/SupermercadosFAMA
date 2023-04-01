#include "../sqLite/sqlite3.h"
#include "../bd/conexion/conexion.h"
#include "../bd/data/getData.h"
#include <stdio.h>
#include <stdlib.h>
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
        // Mostrar menú de cliente
       
    } else if (validarAdministrador(dni, contrasena)) {
        printf("Bienvenido, administrador!\n");
        // Mostrar menú de administrador
    
    } else {
        printf("Usuario o contraseña incorrectos.\n");
    }
    
    sqlite3_close(db);
    return 0;
}

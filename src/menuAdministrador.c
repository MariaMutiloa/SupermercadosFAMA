#include <stdio.h>
#include "menuAdministrador.h"
#include "../bd/data/getData.h"

void admin_menu() {
    char input[256];
    int choice;

    do {
        printf("MENU DE ADMINISTRADOR\n\n");
        printf("1. Añadir clientes a la base de datos\n");
        printf("2. Eliminar cuentas de clientes\n");
        printf("3. Añadir productos mediante compras a los proveedores\n");
        printf("4. Visualizar compras realizadas por todos los clientes\n");
        printf("5. Salir del programa\n\n");
        printf("Opcion a escoger: ");

        fgets(input, sizeof(input), stdin);
        sscanf(input, "%d", &choice);

        switch (choice) {
            case 1:
                printf("Añadir cliente a la base de datos\n");
                agregarCliente();
                break;
            case 2:
                printf("Eliminar cuenta de cliente\n");
                eliminarCliente();
                break;
            case 3:
                printf("Hacer compra a proveedores\n");
                realizarPedido();
                break;
            case 4:
                printf("Visualizar compras realizadas por todos los clientes\n");
                imprimirCompras();
                break;
            case 5:
                printf("Salir del programa\n");
                break;
            default:
                printf("La opcion que has introducido no es correcta. Introduce una opcion valida.\n");
                break;
        }
    } while (choice != 5);
}




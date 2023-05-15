#include <iostream>
#include <cstdlib>
#include "menuCliente.h"
#include "../bd/data/getData.h"

int opc = 0;

void menu()
{
    std::cout << " ================\n";
    std::cout << " MENU DE CLIENTE\n";
    std::cout << " ================\n\n";
    std::cout << " 1. Comprar\n";
    std::cout << " 2. Modificar tus datos\n";
    std::cout << " 3. Mi historial\n";
    std::cout << " 4. Salir del programa\n\n";
    std::cout << "Opcion a escoger: ";
    std::cin >> opc;
}

void cliente_menu()
{
    do
    {
        system("cls");
        menu();
        switch (opc)
        {
            case 1:
                do
                {
                    system("cls");
                    realizarCompra();
                    break;
                } while (opc != 7);
                break;

            case 2:
                do
                {
                    system("cls");
                    actualizarDatosCliente();
                    break;
                } while (opc != 7);
                break;

            case 3:
                do
                {
                    system("cls");
                    imprimirComprasCliente();
                    break;
                } while (opc != 7);
                break;

            case 4:
                do
                {
                    exit(opc = 4);
                } while (opc != 7);
                break;

            case 5:
                break;
            
            default:
                system("cls");
                std::cout << "La opcion que has introducido no es correcta";
                break;
        }
    } while (opc != 7);
}

int main()
{
    cliente_menu();
    return 0;
}

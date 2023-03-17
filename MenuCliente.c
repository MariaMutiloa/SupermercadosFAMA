#include <stdio.h>
		#include <stdlib.h>

		int opc = 0;
		int pro = 0;

	void menu(){
			printf("MENU DE CLIENTE\n\n");
			printf("1. Comprar\n");
			printf("2. Modificar tus datos\n");
			printf("3. Mi historial\n");
			printf("4. Atras\n");
			printf("5. Salir del programa\n\n");
			printf("Opcion a escoger: ");
			scanf("%d", &opc);
	}

	void comprar (){
			printf("Que productos quieres?\n\n");
			printf("Colacao\n");
			printf("Fanta de naranja\n");
			printf("Chocolate\n");
			printf("Redbull\n");
			printf("Cerveza\n");
			printf("Patatas\n");
			printf("Manzana\n\n");
			printf("Producto a escoger: ");
			scanf("%d", &pro);
	}
		
	int main (void){
			

		do{
			system ("cls");
			menu();
			
			switch(opc){
				case 1:
					do{
						system("cls");
						comprar();
					
						case  7:
							do{
								system("cls");
								printf("Has metido Colacao a la cesta");

							}while(pro = 1);
						

				}while(opc != 7);

				
				case 2:

					do{
					system("cls");
					printf("Tu perfil");
					}while(opc != 7);

				case 3: 

					do{
					system("cls");
					printf("Este es tu historial");
					}while(opc != 7);

				case 4: 
						do{
					system("cls");
					printf("LOGIN");
					
				}while(opc != 7);
					

				case 5:
					do{
					exit(opc = 5);
				}while (opc != 7);
				
					
				case 6:
					
					break;
				default:
					system("cls");
					printf("La opcion que has introducido no es correcta");
					break;
			}
		}while(opc != 7);

		return 0;
	}
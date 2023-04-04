	#include <stdio.h>
		#include <stdlib.h>

		int opc = 0;
		int his = 0;

	void menu(){
			printf(" ================\n");
			printf(" MENU DE CLIENTE\n");
			printf(" ================\n\n");
			printf(" 1. Comprar\n");
			printf(" 2. Modificar tus datos\n");
			printf(" 3. Mi historial\n");
			printf(" 4. Salir del programa\n\n");
			printf("Opcion a escoger: ");
			scanf("%d", &opc);
	}


	void historial (){
			printf(" ================\n");
			printf(" TU HISTORIAL\n");
			printf(" ================\n\n");
			printf(" 1. Ver mi Historial \n");
			printf(" 2. Atras \n");
			printf(" Que quieres hacer? ");
			scanf("%d", &his);
	}
	

		
		void cliente_menu(){

		do{
			system ("cls");
			menu();
			switch(opc){
				case 1:
					do{
						system("cls");
						realizarCompra();
						break;
					}while(opc != 7);

				case 2:

					do{
						system("cls");
						actualizarDatosCliente();
						break;
					}while(opc != 7);

				
				case 3: 

					do{
						system("cls");
						historial();
						break;
					}while(opc != 7);

	
				case 4:
					do{
					exit(opc = 4);
				}while (opc != 7);
				
					
				case 5:
					break;
				default:
					system("cls");
					printf("La opcion que has introducido no es correcta");
					break;
			}
		}while(opc != 7);
}
		


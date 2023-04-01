	#include <stdio.h>
		#include <stdlib.h>

		int opc = 0;
		int pro = 0;
		int data = 0;
		int his = 0;

	void menu(){
			printf(" ================\n");
			printf(" MENU DE CLIENTE\n");
			printf(" ================\n\n");
			printf(" 1. Comprar\n");
			printf(" 2. Modificar tus datos\n");
			printf(" 3. Mi historial\n");
			printf(" 4. Atras\n");
			printf(" 5. Salir del programa\n\n");
			printf("Opcion a escoger: ");
			scanf("%d", &opc);
	}

	void comprar (){
			printf(" Que productos quieres?\n\n");
			printf(" 1. Colacao\n");
			printf(" 2. Fanta de naranja\n");
			printf(" 3. Chocolate\n");
			printf(" 4. Redbull\n");
			printf(" 5. Cerveza\n");
			printf(" 6. Patatas\n");
			printf(" 7. Manzana\n");
			printf(" 8. Atras\n\n");
			printf(" Producto a escoger: ");
			scanf("%d", &pro);
	}

	void modificarDatos (){
			printf(" ================\n");
			printf(" TU PERFIL\n");
			printf(" ================\n\n");
			printf(" 1. Nombre de usuario: \n");
			printf(" 2. Contrasenya\n");
			printf(" 3. Atras\n");
			printf(" Dato a que quieres modificar: ");
			scanf("%d", &data);
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
	

		
//	int main (void){
			
		void cliente_menu(){

		do{
			system ("cls");
			menu();
			switch(opc){
				case 1:
					do{
						system("cls");
						comprar();
						switch(pro){
						case  1:
							do{
								//system("cls");
								printf("Has metido Colacao a la cesta");
								
							}while(pro != 1);

						case  2:
							do{
								system("cls");
								printf("Has metido Fanta de Naranja a la cesta");
								
							}while(pro != 9);

						case  3:
							do{
								system("cls");
								printf("Has metido Chocolate a la cesta");
								
							}while(pro != 9);

						case  4:
							do{
								system("cls");
								printf("Has metido Redbull a la cesta");
								
							}while(pro != 9);

						case  5:

							do{
								system("cls");
								printf("Has metido Cerveza a la cesta");
								
							}while(pro != 9);

						case  6:
							do{
								system("cls");
								printf("Has metido Patatas a la cesta");
								
							}while(pro != 9);

						case  7:
							do{
								system("cls");
								printf("Has metido Manzana a la cesta");
								
							}while(pro != 9);

						case  8:
							do{
								system ("cls");
								cliente_menu();
										
							}while(pro != 9);						

						}

				}while(opc != 7);

				
				case 2:

					do{
						system("cls");
						modificarDatos();
						switch(data){

						case  1:
							do{
								//system("cls");
								printf("Este es tu nombre de Perfil");
								
							}while(data != 4);

						case  2:
							do{
								system("cls");
								printf("Esta es tu Contrasenya");
								
							}while(data != 4);

						case  3:
							do{
								system ("cls");
								cliente_menu();
								
							}while(data != 4);
						}
					
					}while(opc != 7);

				
			

				case 3: 

					do{
						system("cls");
						historial();
						switch(his){

						case  1:
							do{
								system("cls");
								printf("Modificar Datos ");
								
							}while(his != 3);

						case  2:
							do{
								system ("cls");
								cliente_menu();
								
							}while(his != 4);
						}
					
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
}
	cliente_menu();
		return 0;

//	}
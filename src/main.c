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
    

//CLIENTES
char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES ("12345678A", "Juan", "Perez", "Calle Falsa 123", "juan.perez@gmail.com", "1111-2222-3333-4444", "password")";
char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES ("56789012B", "Ana", "Garcia", "Av. Siempre Viva 742", "ana.garcia@hotmail.com", "5555-6666-7777-8888", "contraseña")";
char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES ("90123456C", "Pedro", "Jimenez", "Plaza Mayor 1", "pedro.jimenez@yahoo.es", "1234-5678-9101-1121", "123456")";
char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES ("34567890D", "Maria", "Lopez", "Calle Mayor 2", "maria.lopez@gmail.com", "9999-8888-7777-6666", "abcd1234")";
char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES ("09876543E", "Carlos", "Gomez", "Av. Libertador 123", "carlos.gomez@gmail.com", "4321-0987-6543-2109", "qwerty")";
char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES ("21098765F", "Laura", "Sanchez", "Calle del Sol 456", "laura.sanchez@hotmail.com", "7777-6666-5555-4444", "contraseña123")";
char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES ("54321098G", "Antonio", "Martinez", "Plaza de España 5", "antonio.martinez@yahoo.com", "2468-1357-8642-9713", "pass1234")";
char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES ("87654321H", "Marta", "Alvarez", "Calle Nueva 7", "marta.alvarez@gmail.com", "0000-1111-2222-3333", "abcd123456")";
char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES ("23456789I", "Francisco", "Rodriguez", "Av. de la Paz 12", "francisco.rodriguez@gmail.com", "9999-8888-7777-6666", "contraseña123456")";
char *query = "INSERT INTO cliente (dni, Nombre, Apellido, Direccion_Domicilio, Correo_electronico, Tarjeta, Contrasena) VALUES ("98765432J", "Elena", "Fernandez", "Calle de la Luna 23", "elena.fernandez@hotmail.com", "5555-4444-3333-2222", "password123")";
//ADMINISTRADORES
char *query = "INSERT INTO ADMINISTRADOR (dni, contraseña, nombre, apellido, Correo_electronico) VALUES ("12345678A", "Abc123def", "Juan", "Pérez", "juanperez@email.com")";
char *query = "INSERT INTO ADMINISTRADOR (dni, contraseña, nombre, apellido, Correo_electronico) VALUES ("87654321B", "Pass123word", "Ana", "Gómez", "anagomez@email.com")";
char *query = "INSERT INTO ADMINISTRADOR (dni, contraseña, nombre, apellido, Correo_electronico) VALUES ("11223344C", "Qwerty123", "Luis", "Martínez", "luismartinez@email.com")";
char *query = "INSERT INTO ADMINISTRADOR (dni, contraseña, nombre, apellido, Correo_electronico) VALUES ("98765432D", "Password123!", "Elena", "Rodríguez", "elenarodriguez@email.com")";
char *query = "INSERT INTO ADMINISTRADOR (dni, contraseña, nombre, apellido, Correo_electronico) VALUES ("55667788E", "Abcdef1234", "Mario", "Hernández", "mariohernandez@email.com")";
char *query = "INSERT INTO ADMINISTRADOR (dni, contraseña, nombre, apellido, Correo_electronico) VALUES ("33445566F", "123456Abc!", "Laura", "García", "lauragarcia@email.com")";
char *query = "INSERT INTO ADMINISTRADOR (dni, contraseña, nombre, apellido, Correo_electronico) VALUES ("99887766G", "P@ssword123", "Sergio", "Fernández", "sergiofernandez@email.com")";
char *query = "INSERT INTO ADMINISTRADOR (dni, contraseña, nombre, apellido, Correo_electronico) VALUES ("44332211H", "Abcd1234!", "Julia", "Sánchez", "juliasanchez@email.com")";
//PRODUCTO
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("2.99", "001", "Leche entera de vaca, 1 litro")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("1.79", "002", "Huevos frescos, docena")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("4.50", "003", "Pan de molde integral, 500g")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("0.99", "004", "Manzanas, kilo")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("3.25", "005", "Paquete de salchichas, 8 unidades")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("1.99", "006", "Pasta de dientes, 75ml")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("2.50", "007", "Refresco de naranja, 2 litros")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("5.75", "008", "Papel higiénico, paquete de 6 rollos")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("3.99", "009", "Caja de cereales integrales, 500g")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("1.25", "010", "Yogur natural, 125g")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("2.75", "011", "Barra de chocolate negro, 100g")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("1.49", "012", "Bolsa de zanahorias, kilo")";
char *query = "INSERT INTO PRODUCTOS (importe, cod_prod, descripcion) VALUES ("4.99", "013", "Aceite de oliva virgen extra, botella de 500ml")";

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

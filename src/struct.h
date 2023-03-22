#define STRUCT_H
#ifndef STRUCT_H

typedef struct {
char dni [8];
char correo [50];
char num_tarjeta [8];
char direccion[80];
char contrasena[20];
char nombre[15];
char apellido[15];

}Cliente;


typedef struct {
char dni [8];
char correo [50];
char contrasena[20];
char nombre[15];
char apellido[15];

}Administrador;

#endif
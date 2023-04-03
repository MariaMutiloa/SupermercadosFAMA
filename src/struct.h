#ifndef STRUCT_H
#define STRUCT_H

typedef struct {
char dni [10];
char correo [80];
char num_tarjeta [16];
char direccion[80];
char contrasena[20];
char nombre[15];
char apellido[15];
}Cliente;

typedef struct {
char dni [10];
char correo [50];
char contrasena[20];
char nombre[15];
char apellido[15];
}Administrador;

#endif
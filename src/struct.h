#ifndef STRUCT_H
#define STRUCT_H

typedef struct {
char dni [10];
char correo [50];
char contrasena[20];
char nombre[15];
char apellido[15];
}Administrador;

typedef struct {
    int cod_prod;
    char*descripcion;
    int importe;
    int cantidad;
}Producto;

#endif
#define GETDATA_H
#ifndef GETDATA_H
#include "../src/struct.h"

int validarCliente(char dni[], char contrasena[]);
int validarAdministrador(char dni[], char contrasena[]);
void agregarCliente();
void eliminarCliente();
void imprimirCompras();

#endif
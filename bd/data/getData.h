#ifndef GETDATA_H
#define GETDATA_H
#include "../../src/struct.h"
#include "../sqlite3.h"

int validarCliente(char dni[], char contrasena[]);
int validarAdministrador(char dni[], char contrasena[]);
void agregarCliente();
void eliminarCliente();
void imprimirCompras();
int calcularImporte(int cantidad, int cod_prod) ;
int callback(void *data, int argc, char **argv, char **azColName);
void realizarPedido();
void actualizarDatosCliente();

#endif
#ifndef _PEDIDO_H_
#define _PEDIDO_H_
#include <iostream>
using namespace std;

class Pedido
{
private:
int cod_prod;
char*descripcion;
int importe;
int cantidad;
public:
Pedido();
Pedido(int cod_prod, char*descripcion, int importe, int cantidad);
~Pedido();
int getCod_prod();
char* getDescripcion();
int getImporte();
int getCantidad();
void setCod_prod();
void setDescripcion();
void setImporte();
void setCantidad();
};

#endif
#include "pedido.h"
#include <iostream>
using namespace std;
#include <string.h>
#include <stdlib.h>

Pedido :: Pedido(){
    this->cod_prod=0;
    this->descripcion=nullptr;
    this->cantidad=0;
    this->importe=0;
}

Pedido :: Pedido (int cod_prod, char*descripcion, int importe, int cantidad)
{
    this->descripcion = new char[strlen(descripcion) + 1];
    strcpy(this->descripcion, descripcion);
}

Pedido ::~Pedido()
{
    delete[] descripcion;
}

int Pedido:: getCod_prod()
{
    return this->cod_prod;
}

 char* Pedido:: getDescripcion()
{
    return this->descripcion;
}

int Pedido:: getImporte(){
    this->importe = importe;
}

int Pedido:: getCantidad(){
    this->cantidad = cantidad;
}

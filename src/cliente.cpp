#include "persona.h"
#include "cliente.h"
#include <iostream>
using namespace std;
#include <string.h>
#include <stdlib.h>

Cliente :: Cliente(){
    this->tarjeta=nullptr;
    this->direccion=nullptr;
}

Cliente :: Cliente (char* tarjeta, char* direccion)
{
    this->tarjeta = new char[strlen(tarjeta) + 1];
    strcpy(this->tarjeta, tarjeta);
    this->direccion= new char[strlen(direccion)+1];
    strcpy(this->direccion, direccion);
}

Cliente ::~Cliente()
{
    delete[] tarjeta;
    delete[] direccion;
}

char* Cliente:: getTarjeta()
{
    return this->tarjeta;
}

 char* Cliente:: getDireccion()
{
    return this->direccion;
}

void Cliente:: setTarjeta(){
    this->tarjeta = tarjeta;
}

void Cliente:: setDireccion(){
    this->direccion = direccion;
}

void ImprimirPersona(Cliente cliente){
    std::cout << "Su nombre es"<<(cliente.getNombre())<<endl;
    std::cout << "Se apellida"<<(cliente.getApellido())<<endl;
}
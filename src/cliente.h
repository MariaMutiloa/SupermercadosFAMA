#include "persona.h"

#ifndef _CLIETNE_H_
#define _CLIENTE_H_

class Cliente: public Persona
{
private:
char* tarjeta;
char* direccion;
public:
Cliente();
Cliente :: Cliente(char* tarjeta, char* direccion);
char* getTarjeta();
char* getDireccion();
void setTarjeta();
void setDireccion();
virtual void ImprimirPersona();
};

#endif
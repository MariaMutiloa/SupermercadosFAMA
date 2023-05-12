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
Cliente(char* nombre, char* apellido, char* dni, char* correo, char* contrasena,char* tarjeta, char* direccion);
int getTarjeta();
int getDireccion();
void setTarjeta();
void setDireccion();
virtual void ImprimirPersona();
};

#endif
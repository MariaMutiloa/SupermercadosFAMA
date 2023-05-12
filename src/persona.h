#include <iostream>
using namespace std;

#ifndef _PERSONA_H_
#define _PERSONA_H_

class Persona
{
private:
char* nombre;
char* apellido;
char* dni;
char* correo;
char* contrasena;
public:
Persona();
Persona(char* nombre, char* apellido, char* dni, char* correo, char* contrasena);
~Persona();
char* getNombre();
char* getApellido();
char* getDni();
char* getCorreo();
char* getContrasena();
void setNombre();
void setApellido();
void setDni();
void setCorreo();
void setContrasena();
virtual void ImprimirPersona();
};

#endif
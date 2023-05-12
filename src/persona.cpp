#include "persona.h"
#include <iostream>
using namespace std;
#include <string.h>
#include <stdlib.h>

Persona:: Persona(){
    this->nombre = nullptr;
    this->apellido= nullptr;
    this->dni= nullptr;
    this->contrasena= nullptr;
    this->correo=nullptr;
}
Persona::Persona(char* nombre, char* apellido, char* dni, char* correo, char* contrasena )
{
    this->nombre = new char[strlen(nombre) + 1];
    strcpy(this->nombre, nombre);
    this->apellido= new char[strlen(apellido)+1];
    strcpy(this->apellido, apellido);
    this->dni= new char[strlen(dni)+1];
    strcpy(this->dni, dni);
    this->correo= new char[strlen(correo)+1];
    strcpy(this->correo, correo);
    this->contrasena= new char[strlen(contrasena)+1];
    strcpy(this->contrasena, contrasena);
}
Persona::~Persona()
{
    delete[] nombre;
    delete[] apellido;
    delete[] dni;
    delete[] correo;
    delete[] contrasena;
}
char* Persona::getNombre()
{
    return this->nombre;
}
 char* Persona:: getApellido()
{
    return this->apellido;
}

char* Persona:: getContrasena(){
    return this-> contrasena;
}

char* Persona ::getDni(){
    return this-> dni;
}

char* Persona :: getCorreo(){
    return this-> correo;
}

void Persona:: setNombre(){
    this->nombre = nombre;
}

void Persona:: setApellido(){
    this->apellido = apellido;
}

void Persona:: setContrasena(){
    this->contrasena = contrasena;
}

void Persona:: setDni(){
    this->dni = dni;
}

void Persona:: setCorreo(){
    this-> correo= correo;
}
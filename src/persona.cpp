#include <iostream>
#include <cstring>
#include "persona.h"
class Persona {
protected:
    char* nombre;
    char* apellido;
    char* dni;
    char* contrasena;
    char* correo;

public:
    Persona() {
        nombre = nullptr;
        apellido = nullptr;
        dni = nullptr;
        contrasena = nullptr;
        correo = nullptr;
    }

    Persona(const char* nombre, const char* apellido, const char* dni, const char* contrasena, const char* correo) {
        this->nombre = new char[strlen(nombre) + 1];
        strcpy(this->nombre, nombre);

        this->apellido = new char[strlen(apellido) + 1];
        strcpy(this->apellido, apellido);

        this->dni = new char[strlen(dni) + 1];
        strcpy(this->dni, dni);

        this->contrasena = new char[strlen(contrasena) + 1];
        strcpy(this->contrasena, contrasena);

        this->correo = new char[strlen(correo) + 1];
        strcpy(this->correo, correo);
    }

    Persona(const Persona& otraPersona) {
        nombre = new char[strlen(otraPersona.nombre) + 1];
        strcpy(nombre, otraPersona.nombre);

        apellido = new char[strlen(otraPersona.apellido) + 1];
        strcpy(apellido, otraPersona.apellido);

        dni = new char[strlen(otraPersona.dni) + 1];
        strcpy(dni, otraPersona.dni);

        contrasena = new char[strlen(otraPersona.contrasena) + 1];
        strcpy(contrasena, otraPersona.contrasena);

        correo = new char[strlen(otraPersona.correo) + 1];
        strcpy(correo, otraPersona.correo);
    }

    virtual ~Persona() {
        delete[] nombre;
        delete[] apellido;
        delete[] dni;
        delete[] contrasena;
        delete[] correo;
    }

    // Getters y setters
    const char* getNombre() const {
        return nombre;
    }

    const char* getApellido() const {
        return apellido;
    }

    const char* getDNI() const {
        return dni;
    }

    const char* getContrasena() const {
        return contrasena;
    }

    const char* getCorreo() const {
        return correo;
    }

    void setNombre(const char* nombre) {
        delete[] this->nombre;
        this->nombre = new char[strlen(nombre) + 1];
        strcpy(this->nombre, nombre);
    }

    void setApellido(const char* apellido) {
        delete[] this->apellido;
        this->apellido = new char[strlen(apellido) + 1];
        strcpy(this->apellido, apellido);
    }

    void setDNI(const char* dni) {
        delete[] this->dni;
        this->dni = new char[strlen(dni) + 1];
        strcpy(this->dni, dni);
    }

    void setContrasena(const char* contrasena) {
        delete[] this->contrasena;
        this->contrasena = new char[strlen(contrasena) + 1];
        strcpy(this->contrasena, contrasena);
    }

    void setCorreo(const char* correo) {
        delete[] this->correo;
        this->correo = new char[strlen(correo) + 1];
        strcpy(this->correo, correo);
    }

    virtual void imprimirCliente() = 0;  // Método polimórfico puro (clase abstracta)
};

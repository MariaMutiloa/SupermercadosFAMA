#ifndef PERSONA_H
#define PERSONA_H

class Persona {
protected:
    char* nombre;
    char* apellido;
    char* dni;
    char* contrasena;
    char* correo;

public:
    Persona();
    Persona(const char* nombre, const char* apellido, const char* dni, const char* contrasena, const char* correo);
    Persona(const Persona& otraPersona);
    virtual ~Persona();

    const char* getNombre() const;
    const char* getApellido() const;
    const char* getDNI() const;
    const char* getContrasena() const;
    const char* getCorreo() const;

    void setNombre(const char* nombre);
    void setApellido(const char* apellido);
    void setDNI(const char* dni);
    void setContrasena(const char* contrasena);
    void setCorreo(const char* correo);

    virtual void imprimirCliente() = 0;
};

#endif  // PERSONA_H

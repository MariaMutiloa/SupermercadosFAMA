#include <iostream>
#include <cstring>
#include "persona.h"

class Cliente : public Persona {
private:
    char* num_tarjeta;
    char* direccion;

public:
    Cliente();
    Cliente(const char* nombre, const char* apellido, const char* dni, const char* contrasena, const char* correo,
            const char* num_tarjeta, const char* direccion);
    Cliente(const Cliente& otroCliente);
    ~Cliente();

    const char* getNumTarjeta() const;
    const char* getDireccion() const;

    void setNumTarjeta(const char* num_tarjeta);
    void setDireccion(const char* direccion);

    void imprimirCliente();
};

#include "Cliente.h"

Cliente::Cliente() : Persona()
{
    num_tarjeta = nullptr;
    direccion = nullptr;
}

Cliente::Cliente(const char *nombre, const char *apellido, const char *dni, const char *contrasena, const char *correo,
                 const char *num_tarjeta, const char *direccion) : Persona(nombre, apellido, dni, contrasena, correo)
{
    setNumTarjeta(num_tarjeta);
    setDireccion(direccion);
}

Cliente::Cliente(const Cliente &otroCliente) : Persona(otroCliente)
{
    setNumTarjeta(otroCliente.num_tarjeta);
    setDireccion(otroCliente.direccion);
}

Cliente::~Cliente()
{
    delete[] num_tarjeta;
    delete[] direccion;
}

const char *Cliente::getNumTarjeta() const
{
    return num_tarjeta;
}

const char *Cliente::getDireccion() const
{
    return direccion;
}

void Cliente::setNumTarjeta(const char *num_tarjeta)
{
    delete[] this->num_tarjeta;
    this->num_tarjeta = new char[strlen(num_tarjeta) + 1];
    strcpy(this->num_tarjeta, num_tarjeta);
}

void Cliente::setDireccion(const char *direccion)
{
    delete[] this->direccion;
    this->direccion = new char[strlen(direccion) + 1];
    strcpy(this->direccion, direccion);
}

void Cliente::imprimirCliente()
{
    std::cout << "Nombre: " << getNombre() << std::endl;
    std::cout << "Apellido: " << getApellido() << std::endl;
    std::cout << "DNI: " << getDNI() << std::endl;
}

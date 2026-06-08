#pragma once

#include <iostream>
#include <cstring>
#include "Personaje.h"


struct RegistroPersonaje {
    int id;
    int idArmaEspecial;

    char nombre[50];

    float vida;
    float armadura;
    float cooldownHabilidad;
    float velocidad;
};

class archivoPersonaje {
private:
    char _nombre[20];

    bool verificarArchivo(const char* nombreArchivo);

public:

    archivoPersonaje(const char* nombre);

    bool grabarRegistroPersonaje(RegistroPersonaje objeto);

    int buscarPersonaje(int idPersonaje);

    bool sobreEscribirRegistro(RegistroPersonaje registro, int posicion);

    RegistroPersonaje traerRegistro (int posicion);

    //void entregarPersonaje(std::vector<Arma>& inventario, int idArma);

    void listar();
};

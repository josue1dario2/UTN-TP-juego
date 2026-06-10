#pragma once

#include <iostream>
#include <cstring>
#include "Arma.h"


struct RegistroArma {
    int id;

    char nombre[50];

    float cadencia;
    float danio;
    float alcance;
    float costo;

    int municionMaxima;
    int tamanioCargador;
};

class archivoArma {
private:
    char _nombre[20];

    bool verificarArchivo(const char* nombreArchivo);

public:

    archivoArma(const char* nombre);

    bool grabarRegistroArma(RegistroArma objeto);

    int buscarArma(int idArma);

    bool sobreEscribirRegistro(RegistroArma registro, int posicion);

    RegistroArma traerRegistro (int posicion);

    void entregarArma(std::vector<Arma>& inventario, int idArma);

    void listar();
};

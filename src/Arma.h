#pragma once

#include <string>
#include "ArmaDatos.h"

class Arma {
private:
    std::string nombre;
    int danoBase;
    float cadencia;
    float alcance;
    std::string tipo;

    int balaActual;
    int tamanoMagazine;
    int balasReserva;
    int balasTotales;

    float cooldownDisparo;
    float tiempoRecarga;
    float timerRecarga;
    bool recargando;

public:
    Arma(const ArmaDatos& data);

    bool puedeDisparar() const;
    void disparar();
    void recargar();
    void update(float dt);

    int getDano() const;
    float getAlcance() const;
    const std::string& getNombre() const;
    int getBalasEnMagazine() const;
    int getBalasTotales() const;
    bool estaRecargando() const;
    float getCadencia() const;
    const std::string& getTipo() const;
};
#pragma once

#include "ObjetoGrafico.h"

class Entidad : public ObjetoGrafico {
protected:

    float vidaMax;
    float vidaActual;
    float velocidad;
    int direccion; // 1.izq 2.arriba 3.der 4.abajo

public:

    Entidad();

    virtual void recibirDanio(float cantidad);
    bool estaVivo() const;

    float getVidaActual() const { return vidaActual; }
    void setVidaActual(float v) { vidaActual = v; }
    float getVidaMax() const { return vidaMax; }
    float getVelocidad() const { return velocidad; }
};
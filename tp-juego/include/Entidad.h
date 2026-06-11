#pragma once

#include "ObjetoGrafico.h"

class Entidad : public ObjetoGrafico {
protected:

    float vidaMax;
    float vidaActual;
    float velocidad;

public:

    Entidad();

    virtual void recibirDanio(float cantidad);
    bool estaVivo() const;

    float getVidaActual() const { return vidaActual; }
    float getVidaMax() const { return vidaMax; }
    float getVelocidad() const { return velocidad; }
};
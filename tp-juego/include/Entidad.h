#pragma once

#include "ObjetoGrafico.h"

class Entidad : public ObjetoGrafico {
protected:

    float vidaMax;
    float vidaActual;
    float velocidad;

public:

    Entidad();

    /*
    virtual void recibirDanio(int cantidad);

    bool estaVivo() const;
    */
};
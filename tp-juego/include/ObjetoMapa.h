#pragma once

#include "ObjetoGrafico.h"

class ObjetoMapa : public ObjetoGrafico
{
private:

    bool solido;

public:

    ObjetoMapa();

    void setSolido(bool estado);

    bool esSolido() const;
};
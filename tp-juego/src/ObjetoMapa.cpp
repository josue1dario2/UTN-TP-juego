#include "../include/ObjetoMapa.h"

ObjetoMapa::ObjetoMapa()
{
    solido = true;
}

void ObjetoMapa::setSolido(bool estado)
{
    solido = estado;
}

bool ObjetoMapa::esSolido() const
{
    return solido;
}
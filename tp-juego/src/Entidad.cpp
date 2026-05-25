#include "../include/Entidad.h"

Entidad::Entidad()
{
    vidaMax = 100;
    vidaActual = vidaMax;
}

/*
void Entidad::recibirDanio(int cantidad)
{
    vidaActual -= cantidad;

    if (vidaActual < 0)
    {
        vidaActual = 0;
    }
}

bool Entidad::estaVivo() const
{
    return vidaActual > 0;
}
*/
#pragma once
// Enemigo.h - Clase base abstracta para cualquier enemigo

#include "Entidad.h"

class Enemigo : public Entidad {
public:
    Enemigo(sf::Texture& textura);
    virtual ~Enemigo();

    // Metodos especificos de enemigo
    virtual void atacar() = 0;
    virtual void quitarVida(int cantidad) = 0;
    virtual bool muerto() const = 0;
    virtual int getDano() const = 0;
    virtual bool puedeAtacar() const = 0;

    // Configurar el objetivo (jugador) al que este enemigo persigue
    virtual void setObjetivo(sf::Vector2f objetivo) = 0;
};

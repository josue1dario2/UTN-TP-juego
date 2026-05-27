#pragma once
// Bala.h - Proyectil disparado por el jugador

#include "Entidad.h"

class Bala : public Entidad {
private:
    sf::Vector2f direccion;
    int dano;
    float alcanceMaximo;
    float distanciaRecorrida;

public:
    Bala(sf::Vector2f posicion, sf::Vector2f direccion, float velocidad,
           int dano, float alcance, sf::Texture& textura);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& ventana) override;

    int getDano() const;
    bool expiro() const;
    sf::FloatRect getBounds() const;
};

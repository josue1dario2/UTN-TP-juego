#pragma once
#include "ObjetoGrafico.h"

class Proyectil : public ObjetoGrafico {
private:
    sf::Vector2f direccion;
    float velocidad;
    float alcanceMax;
    float distanciaRecorrida;
    bool activo;
    sf::CircleShape formaBala;

public:
    Proyectil(sf::Vector2f posInicial, sf::Vector2f dir, float alc = 600.f, float vel = 800.f);

    void actualizar(float deltaTime) override;
    void dibujar(sf::RenderWindow& ventana) override;

    void verificarColisiones(const sf::Image& mapaColision);
    bool isActivo() const;
};

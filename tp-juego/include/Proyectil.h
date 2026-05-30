#pragma once
#include "ObjetoGrafico.h"

class Proyectil : public ObjetoGrafico {
private:
    sf::Vector2f direccion;
    float velocidad;
    float alcanceMax;
    float distanciaRecorrida;
    bool activo;

public:
    Proyectil(sf::Vector2f posInicial, sf::Vector2f dir, float alc = 600.f, float vel = 800.f);

    void actualizar(float deltaTime) override;

    sf::Vector2f getPosicion() const {
        return direccion;
    }

    bool isActivo() const;
};

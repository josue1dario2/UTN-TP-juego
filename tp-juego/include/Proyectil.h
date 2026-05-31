#pragma once
#include "ObjetoGrafico.h"
#include "ObjetoMapa.h"

class Proyectil : public ObjetoGrafico {
private:

    sf::Vector2f direccion;

    float velocidad;
    float alcanceMax;
    float danio;

    float distanciaRecorrida;

    bool estadoActivo;

public:
    Proyectil(sf::Texture& textura, sf::Vector2f posInicial, sf::Vector2f dir, float alc, float vel, float danio);

    void actualizar(float deltaTime, const std::vector<ObjetoMapa>& obstaculos);

    bool debeDestruirse() const;
};

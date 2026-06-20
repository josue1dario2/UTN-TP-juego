#pragma once;
#include "ObjetoGrafico.h"

class Mina : public ObjetoGrafico {
private:
    bool activa;
    float danio;
    float radioExplosion;

public:

    void actualizar(float deltaTime, const std::vector<sf::FloatRect> &hitboxZombies);
    void explotar();
};
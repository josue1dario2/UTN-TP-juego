#pragma once
#include "ObjetoGrafico.h"
#include <iostream>

class zombie;

class Mina : public ObjetoGrafico {
private:
    bool activa;
    bool destruir;
    bool exploto;

    float danio;
    float radioExplosion;
    float temporizador;

public:

    Mina(sf::Vector2f posicion);

    void actualizar(float deltaTime, const std::vector<sf::FloatRect> &listaZombies);

    void explotar();

    bool debeDestruirse() const {return destruir;}

    bool getExplosion() const {return exploto;}

    float getDanio() const {return danio;}
};
#pragma once
#include "ObjetoGrafico.h"

class Puntero : public ObjetoGrafico {
public:
    Puntero();

    // Actualiza la posición y rota la mira según el tiempo transcurrido
    void actualizar(sf::RenderWindow& ventana, float deltaTime);
};

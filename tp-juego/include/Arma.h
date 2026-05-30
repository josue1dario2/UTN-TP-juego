#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Proyectil.h"

class Arma : public ObjetoGrafico {
private:
    int idArma;
    std::string nombre;
    int danio;

    int municionActual;

    float cadencia;
    float alcance;
    float costo;

public:
    Arma();
    Arma(int id, std::string nom, int dmg, float alc, float cost);
    void actualizar(float deltaTime, const sf::Vector2f &posicionMouse, const sf::Vector2f& posicionJugador, std::vector<Proyectil>& proyectiles);

    // Getters y Setters
    int getIdArma() const;
    std::string getNombre() const;
    int getDanio() const;
    float getAlcance() const;
    float getCosto() const;
};

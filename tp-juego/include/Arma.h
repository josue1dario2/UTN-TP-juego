#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Proyectil.h"

class Arma {
private:
    int idArma;
    std::string nombre;
    int danio;
    float alcance;
    float costo;

    // Vector de objetos Proyectil de acuerdo con las clases del proyecto
    std::vector<Proyectil> proyectiles;

public:
    Arma();
    Arma(int id, std::string nom, int dmg, float alc, float cost);

    // Getters y Setters
    int getIdArma() const;
    std::string getNombre() const;
    int getDanio() const;
    float getAlcance() const;
    float getCosto() const;

    // Métodos del diagrama UML
    void disparar(sf::Vector2f origen, sf::Vector2f destino);
    void recargar();

    // Métodos de actualización y dibujo de proyectiles
    void actualizar(float deltaTime);
    void dibujar(sf::RenderWindow& ventana);
};

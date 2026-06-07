#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Proyectil.h"

class Arma : public ObjetoGrafico {
private:
    int idArma;
    std::string nombre;
    
    int municionActual;
    
    float tiempoDesdeUltimoDisparo;

    float cadencia;
    float danio;
    float alcance;
    float costo;
    int municionMaxima;
    int tamanioCargador;

    void disparoEscopeta(float deltaX, float deltaY, std::vector<Proyectil>& proyectiles, sf::Texture& texturaProyectil);

public:
    Arma();
    Arma(int id, std::string nombre, float cadencia, float danio, float alcance, float costo, int municionMaxima, int tamanioCargador);

    virtual void actualizar(float deltaTime, const sf::Vector2f &posicionMouse, const sf::Vector2f& posicionJugador, 
        std::vector<Proyectil>& proyectiles, sf::Texture& texturaProyectil);

    // Getters y Setters
    int getIdArma() const;
    std::string getNombre() const;
    float getDanio() const;
    float getAlcance() const;
    float getCosto() const;

    bool estaDisponible() const; // Verifica si el arma está disponible para ser equipada
};

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Entidad.h"


class Personaje : public Entidad {
private:
    int idPersonaje;
    std::string nombre;
    float armaduraMax;
    float armaduraActual;
    
    std::string habilidad;

    // Sistema de colisiones por mapa
    const sf::Image* mapaColision;
    bool esPosicionValida(float x, float y) const;

public:

    Personaje();

    void cargarAtributos(int id, std::string nom, float vida, float armadura, float vel, std::string hab);
    void setMapaColision(const sf::Image* mapa);

    void actualizar(float deltaTime) override;

    void controlar(float movimiento);   // Movimiento con W, A, S, D
};
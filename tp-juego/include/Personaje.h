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

    sf::Vector2f posicionAnterior;
    float movimientoX;
    float movimientoY;

public:

    Personaje();

    void cargarAtributos(int id, std::string nom, float vida, float armadura, float vel, std::string hab);

    void actualizar(float deltaTime) override;

    //void controlar(float movimiento);   // Movimiento con W, A, S, D

    void guardarPosicionAnterior();

    void volverPosicionAnteriorX();

    void volverPosicionAnteriorY();

    float getMovimientoX() const;

    float getMovimientoY() const;
};
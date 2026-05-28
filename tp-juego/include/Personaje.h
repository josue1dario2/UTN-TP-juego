#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Entidad.h"
#include "Arma.h"


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

    // Sistema de colisiones por mapa
    const sf::Image* mapaColision;

    // Arma equipada del jugador
    Arma armaEquipada;


public:

    Personaje();

    void cargarAtributos(int id, std::string nom, float vida, float armadura, float vel, std::string hab);
    void setMapaColision(const sf::Image* mapa);
    bool esPosicionValida(float x, float y) const;
    Arma& getArma();

    void actualizar(float deltaTime) override;
    void controlar(float movimiento);

    //void controlar(float movimiento);   // Movimiento con W, A, S, D

    void guardarPosicionAnterior();

    void volverPosicionAnteriorX();

    void volverPosicionAnteriorY();

    float getMovimientoX() const;

    float getMovimientoY() const;
};
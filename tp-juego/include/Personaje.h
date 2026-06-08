#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Entidad.h"
#include "Arma.h"
#include "ObjetoMapa.h"
#include "Proyectil.h"
#include "archivoArma.h"


class Personaje : public Entidad {
private:
    int idPersonaje;
    std::string nombre;
    float armaduraMax;
    float armaduraActual;
    float cooldownHabilidad;
    std::string habilidad;


    sf::Vector2f posicionAnterior;
    float movimientoX;
    float movimientoY;

    // Arma equipada del jugador
    std::vector<Arma> inventarioArmas;
    int armaEquipada; // Índice del arma actualmente equipada en el inventario


public:

    Personaje();

    Personaje(int id, int idArmaEspecial, std::string nombre, float vida, float armadura, float velocidad, float cooldownHabilidad);

    virtual void actualizar(float deltaTime, const std::vector<ObjetoMapa>& obstaculos);
    void controlar(float movimiento);

    void guardarPosicionAnterior();

    void volverPosicionAnteriorX();

    void volverPosicionAnteriorY();

    float getMovimientoX() const;

    float getMovimientoY() const;

    Arma& getArma();

    void elegirArma();
};
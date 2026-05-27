#pragma once
// Zombie.h - Clase zombie segun diagrama UML

#include "Entidad.h"

enum class TipoZombie {
    NORMAL,
    RAPIDO,
    PESADO
};

class Zombie : public Entidad {
private:
    int tipo;
    int vida;
    int ataque;
    float velocidad;

    int dineroAlMorir;
    float cooldownAtaque;
    float timerAtaque;
    sf::Vector2f posicionObjetivo;
    float escalaBase;
    int anchoFrame;
    int altoFrame;
    float timerCaminar;

public:
    Zombie(sf::Texture& textura, sf::Vector2f posicionSpawn, TipoZombie zTipo);
    ~Zombie();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& ventana) override;

    // Segun diagrama UML
    void atacar();
    void quitarVida(int cantidad);
    bool muerto() const;

    // Metodos que necesita Juego.cpp
    void moverHacia(sf::Vector2f objetivo, float deltaTime);
    void setObjetivo(sf::Vector2f objetivo);
    bool puedeAtacar() const;
    int getDano() const;
    int getDineroAlMorir() const;
    TipoZombie getTipo() const;
    bool esPosicionValida(sf::Vector2f nuevaPos) const;
};

#pragma once
// Zombie.h - Clase enemigo zombie

#include "Entidad.h"

// Tipos de zombie
enum class TipoZombie {
    NORMAL,  // Zombie estandar: balance vida/velocidad/dano
    RAPIDO,    // Zombie rapido: poca vida pero muy veloz
    PESADO    // Zombie pesado: mucha vida y dano pero lento
};

class Zombie : public Entidad {
private:
    TipoZombie tipo;
    int dano;
    int dineroAlMorir;
    float cooldownAtaque;
    float timerAtaque;

    sf::Vector2f posicionObjetivo;

    float timerCaminar;
    float escalaBase;
    int anchoFrame;
    int altoFrame;

public:
    Zombie(sf::Texture& textura, sf::Vector2f posicionSpawn, TipoZombie tipo);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& ventana) override;

    void moverHacia(sf::Vector2f objetivo, float deltaTime);
    void setObjetivo(sf::Vector2f objetivo);
    bool puedeAtacar() const;
    void atacar();

    int getDano() const;
    int getDineroAlMorir() const;
    TipoZombie getTipo() const;
};

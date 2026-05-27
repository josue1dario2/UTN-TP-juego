// Zombie.cpp - Implementacion de Zombie segun diagrama UML

#include "Zombie.h"
#include "Mapa.h"
#include <cmath>
#include <iostream>

Zombie::Zombie(sf::Texture& textura, sf::Vector2f posicionSpawn, TipoZombie zTipo)
    : Entidad(textura),
      tipo(static_cast<int>(zTipo)),
      vida(100),
      ataque(10),
      velocidad(40.f),
      dineroAlMorir(50),
      cooldownAtaque(1.0f),
      timerAtaque(0.f),
      posicionObjetivo(posicionSpawn),
      escalaBase(1.0f),
      timerCaminar(0.f)
{
    this->posicion = posicionSpawn;
    this->vivo = true;

    sprite.setTexture(textura);

    sf::Vector2u texSize = textura.getSize();
    anchoFrame = texSize.x;
    altoFrame = texSize.y;

    sprite.setTextureRect(sf::IntRect(0, 0, anchoFrame, altoFrame));
    sprite.setOrigin(anchoFrame / 2.0f, altoFrame / 2.0f);

    if (anchoFrame > 0) {
        escalaBase = 160.f / static_cast<float>(anchoFrame);
    }

    switch (zTipo) {
        case TipoZombie::NORMAL:
            vida = 100;
            ataque = 10;
            velocidad = 40.f;
            dineroAlMorir = 50;
            cooldownAtaque = 1.0f;
            sprite.setColor(sf::Color(50, 200, 50));
            escalaBase = 70.f / static_cast<float>(anchoFrame);
            break;

        case TipoZombie::RAPIDO:
            vida = 60;
            ataque = 5;
            velocidad = 80.f;
            dineroAlMorir = 75;
            cooldownAtaque = 0.6f;
            sprite.setColor(sf::Color(180, 230, 50));
            escalaBase = 60.f / static_cast<float>(anchoFrame);
            break;

        case TipoZombie::PESADO:
            vida = 250;
            ataque = 25;
            velocidad = 25.f;
            dineroAlMorir = 150;
            cooldownAtaque = 1.5f;
            sprite.setColor(sf::Color(20, 100, 20));
            escalaBase = 90.f / static_cast<float>(anchoFrame);
            break;
    }

    salud = vida;
    saludMaxima = vida;
    sprite.setScale(escalaBase, escalaBase);
    sprite.setPosition(posicion);
}

Zombie::~Zombie() {
}

void Zombie::update(float deltaTime) {
    if (!vivo) return;
    moverHacia(posicionObjetivo, deltaTime);

    if (timerAtaque > 0.f) {
        timerAtaque -= deltaTime;
        if (timerAtaque < 0.f) timerAtaque = 0.f;
    }
}

void Zombie::render(sf::RenderWindow& ventana) {
    if (vivo) {
        ventana.draw(sprite);
    }
}

void Zombie::moverHacia(sf::Vector2f objetivo, float deltaTime) {
    sf::Vector2f direccion = objetivo - posicion;
    float magnitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);

    if (magnitud > 5.f) {
        sf::Vector2f normalizada(direccion.x / magnitud, direccion.y / magnitud);
        sf::Vector2f nuevaPos = posicion + normalizada * velocidad * deltaTime;

        if (esPosicionValida(nuevaPos)) {
            posicion = nuevaPos;
        }

        sprite.setPosition(posicion);

        timerCaminar += velocidad * deltaTime * 0.1f;
        float wobbleAngle = std::sin(timerCaminar) * 15.f;
        sprite.setRotation(wobbleAngle);

        float bounce = 1.0f + std::abs(std::sin(timerCaminar)) * 0.1f;
        float actualScaleX = (direccion.x < 0) ? -escalaBase : escalaBase;
        sprite.setScale(actualScaleX, escalaBase * bounce);
    } else {
        sprite.setRotation(0.f);
        timerCaminar += deltaTime * 2.5f;
        float stretch = 1.0f + std::sin(timerCaminar) * 0.05f;
        float squash = 1.0f - std::sin(timerCaminar) * 0.05f;

        float actualScaleX = (sprite.getScale().x < 0) ? -escalaBase : escalaBase;
        sprite.setScale(actualScaleX * stretch, escalaBase * squash);
    }
}

void Zombie::setObjetivo(sf::Vector2f objetivo) {
    posicionObjetivo = objetivo;
}

// === Metodos UML ===
void Zombie::atacar() {
    timerAtaque = cooldownAtaque;
}

void Zombie::quitarVida(int cantidad) {
    vida -= cantidad;
    if (vida <= 0) {
        vida = 0;
        vivo = false;
        salud = 0;
    }
}

bool Zombie::muerto() const {
    return vida <= 0;
}

// === Getters y utilidades ===
bool Zombie::puedeAtacar() const {
    return timerAtaque <= 0.f;
}

int Zombie::getDano() const {
    return ataque;
}

int Zombie::getDineroAlMorir() const {
    return dineroAlMorir;
}

TipoZombie Zombie::getTipo() const {
    return static_cast<TipoZombie>(tipo);
}

bool Zombie::esPosicionValida(sf::Vector2f nuevaPos) const {
    if (mapa) {
        return mapa->esPosicionValida(nuevaPos.x, nuevaPos.y);
    }
    return true;
}

// Zombie.cpp - Clase enemigo zombie

#include "Zombie.h"
#include "Mapa.h"
#include <cmath>

Zombie::Zombie(sf::Texture& textura, sf::Vector2f posicionSpawn, TipoZombie zTipo)
    : Entidad(textura),
      tipo(zTipo),
      dano(10),
      dineroAlMorir(50),
      cooldownAtaque(1.0f),
      timerAtaque(0.f),
      posicionObjetivo(posicionSpawn),
      timerCaminar(0.f),
      escalaBase(1.0f)
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

    switch (tipo) {
        case TipoZombie::NORMAL:
            salud = 100;
            saludMaxima = 100;
            velocidad = 40.f;
            dano = 10;
            dineroAlMorir = 50;
            cooldownAtaque = 1.0f;
            sprite.setColor(sf::Color(50, 200, 50));
            {
                if (anchoFrame > 0) {
                    escalaBase = 70.f / static_cast<float>(anchoFrame);
                    sprite.setScale(escalaBase, escalaBase);
                }
            }
            break;

        case TipoZombie::RAPIDO:
            salud = 60;
            saludMaxima = 60;
            velocidad = 80.f;
            dano = 5;
            dineroAlMorir = 75;
            cooldownAtaque = 0.6f;
            sprite.setColor(sf::Color(180, 230, 50));
            {
                if (anchoFrame > 0) {
                    escalaBase = 60.f / static_cast<float>(anchoFrame);
                    sprite.setScale(escalaBase, escalaBase);
                }
            }
            break;

        case TipoZombie::PESADO:
            salud = 250;
            saludMaxima = 250;
            velocidad = 25.f;
            dano = 25;
            dineroAlMorir = 150;
            cooldownAtaque = 1.5f;
            sprite.setColor(sf::Color(20, 100, 20));
            {
                if (anchoFrame > 0) {
                    escalaBase = 90.f / static_cast<float>(anchoFrame);
                    sprite.setScale(escalaBase, escalaBase);
                }
            }
            break;
    }

    sprite.setPosition(posicion);
}

void Zombie::moverHacia(sf::Vector2f objetivo, float deltaTime) {
    sf::Vector2f direccion = objetivo - posicion;
    float magnitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);

    if (magnitud > 5.f) {
        sf::Vector2f normalizada(direccion.x / magnitud, direccion.y / magnitud);
        
        // Calcular nueva posición
        sf::Vector2f nuevaPos = posicion + normalizada * velocidad * deltaTime;
        
        // Verificar colisión con mapa
        if (esPosicionValida(nuevaPos)) {
            posicion = nuevaPos;
        }
        
        sprite.setPosition(posicion);

        // Animación de Caminar (Wobble/Rotación procedimental)
        timerCaminar += velocidad * deltaTime * 0.1f;
        float wobbleAngle = std::sin(timerCaminar) * 15.f; // Rotar levemente
        sprite.setRotation(wobbleAngle);
        
        // Efecto de rebote
        float bounce = 1.0f + std::abs(std::sin(timerCaminar)) * 0.1f;
        
        // En lugar de rotar al zombie, lo volteamos horizontalmente si el jugador está a la izquierda
        float actualScaleX = (direccion.x < 0) ? -escalaBase : escalaBase;
        sprite.setScale(actualScaleX, escalaBase * bounce);
    } else {
        sprite.setRotation(0.f);
        // Idle animation (Street Fighter style breathing)
        timerCaminar += deltaTime * 2.5f;
        float stretch = 1.0f + std::sin(timerCaminar) * 0.05f;
        float squash = 1.0f - std::sin(timerCaminar) * 0.05f;
        
        float actualScaleX = (sprite.getScale().x < 0) ? -escalaBase : escalaBase;
        sprite.setScale(actualScaleX * stretch, escalaBase * squash);
    }
}

void Zombie::update(float deltaTime) {
    if (!vivo) return;
    moverHacia(posicionObjetivo, deltaTime);

    if (timerAtaque > 0.f) {
        timerAtaque -= deltaTime;
        if (timerAtaque < 0.f) {
            timerAtaque = 0.f;
        }
    }
}

void Zombie::render(sf::RenderWindow& ventana) {
    if (vivo) {
        ventana.draw(sprite);
    }
}

void Zombie::setObjetivo(sf::Vector2f objetivo) {
    posicionObjetivo = objetivo;
}

bool Zombie::puedeAtacar() const {
    return timerAtaque <= 0.f;
}

void Zombie::atacar() {
    timerAtaque = cooldownAtaque;
}

int Zombie::getDano() const {
    return dano;
}

int Zombie::getDineroAlMorir() const {
    return dineroAlMorir;
}

TipoZombie Zombie::getTipo() const {
    return tipo;
}

// Bala.cpp - Proyectil disparado por el jugador

#include "Bala.h"
#include <cmath>

Bala::Bala(sf::Vector2f pos, sf::Vector2f dir, float velocidad,
               int dano, float alcance, sf::Texture& textura)
    : Entidad(textura),
      dano(dano),
      alcanceMaximo(alcance),
      distanciaRecorrida(0.f)
{
    this->posicion = pos;
    this->velocidad = velocidad;
    this->salud = 1;
    this->saludMaxima = 1;
    this->vivo = true;

    float magnitud = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (magnitud > 0.f) {
        direccion = sf::Vector2f(dir.x / magnitud, dir.y / magnitud);
    } else {
        direccion = sf::Vector2f(1.f, 0.f);
    }

    sf::Vector2u texSize = textura.getSize();
    if (texSize.x > 0 && texSize.y > 0) {
        float scaleX = 8.f / static_cast<float>(texSize.x);
        float scaleY = 8.f / static_cast<float>(texSize.y);
        sprite.setScale(scaleX, scaleY);
    }

    sprite.setOrigin(
        static_cast<float>(texSize.x) / 2.f,
        static_cast<float>(texSize.y) / 2.f
    );

    float angleRad = std::atan2(direccion.y, direccion.x);
    float angleDeg = angleRad * (180.f / 3.14159265f);
    sprite.setRotation(angleDeg);

    sprite.setPosition(posicion);
}

void Bala::update(float deltaTime) {
    if (!vivo) return;

    sf::Vector2f movimiento = direccion * velocidad * deltaTime;
    posicion += movimiento;
    sprite.setPosition(posicion);

    float frameDistance = std::sqrt(movimiento.x * movimiento.x + movimiento.y * movimiento.y);
    distanciaRecorrida += frameDistance;

    if (distanciaRecorrida >= alcanceMaximo) {
        vivo = false;
        return;
    }

    const float margen = 50.f;
    if (posicion.x < -margen || posicion.x > 1920.f + margen ||
        posicion.y < -margen || posicion.y > 1280.f + margen) {
        vivo = false;
    }
}

void Bala::render(sf::RenderWindow& ventana) {
    if (vivo) {
        ventana.draw(sprite);
    }
}

int Bala::getDano() const {
    return dano;
}

bool Bala::expiro() const {
    return !vivo;
}

sf::FloatRect Bala::getBounds() const {
    return sprite.getGlobalBounds();
}

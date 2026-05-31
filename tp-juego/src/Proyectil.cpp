#include "Proyectil.h"
#include <cmath>

Proyectil::Proyectil(sf::Texture& texturaProyectil,sf::Vector2f posInicial, sf::Vector2f dir, float alc, float vel) {
    setPosicionCentrado(posInicial.x, posInicial.y);
    sprite.setTexture(texturaProyectil);
    escalarSprite(1.5f, 1.5f);
    mostrarHitbox = false;
    
    velocidad = vel;
    alcanceMax = alc;
    distanciaRecorrida = 0.f;
    direccion.x = dir.x - posInicial.x;
    direccion.y = dir.y - posInicial.y;

    float longitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
    direccion.x /= longitud;
    direccion.y /= longitud;

    setHitbox(5.f, 5.f); // Hitbox pequeña para el proyectil

    setAngulo(std::atan2(direccion.y, direccion.x) * 180.f / 3.14159f);
}

bool Proyectil::debeDestruirse() const {
    return distanciaRecorrida >= alcanceMax;
}

void Proyectil::actualizar(float deltaTime) {

    float desplazamiento = velocidad * deltaTime;
    mover(direccion.x * desplazamiento, direccion.y * desplazamiento);
    distanciaRecorrida += desplazamiento;
}

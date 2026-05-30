#include "Proyectil.h"
#include <cmath>

Proyectil::Proyectil(sf::Vector2f posInicial, sf::Vector2f dir, float alc, float vel) {
    setPosicionCentrado(posInicial.x, posInicial.y);
    
    velocidad = vel;
    alcanceMax = alc;
    distanciaRecorrida = 0.f;
    direccion.x = dir.x - posInicial.x;
    direccion.y = dir.y - posInicial.y;

    float longitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
    direccion.x /= longitud;
    direccion.y /= longitud;
    
    activo = true;

    setHitbox(10.f, 10.f); // Hitbox pequeña para el proyectil
}

bool Proyectil::debeDestruirse() const {
    return distanciaRecorrida >= alcanceMax;
}

void Proyectil::actualizar(float deltaTime) {

    float desplazamiento = velocidad * deltaTime;
    mover(direccion.x * desplazamiento, direccion.y * desplazamiento);
    distanciaRecorrida += desplazamiento;
}

bool Proyectil::isActivo() const {
    return activo;
}

#include "Proyectil.h"

Proyectil::Proyectil(sf::Vector2f posInicial, sf::Vector2f dir, float alc, float vel) : ObjetoGrafico() {
    direccion = dir;
    velocidad = vel;
    alcanceMax = alc;
    distanciaRecorrida = 0.f;
    activo = true;


    setPosicionCentrado(posInicial.x, posInicial.y);
    setHitbox(10.f, 10.f); // Hitbox pequeña para el proyectil
}

void Proyectil::actualizar(float deltaTime) {
    if (!activo) return;

    float paso = velocidad * deltaTime;
    mover(direccion.x * paso, direccion.y * paso);
    distanciaRecorrida += paso;

    // Si supera el alcance máximo, se desactiva
    if (distanciaRecorrida >= alcanceMax) {
        activo = false;
    }
}

bool Proyectil::isActivo() const {
    return activo;
}

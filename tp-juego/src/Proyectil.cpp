#include "Proyectil.h"
#include <cmath>
#include <iostream>

Proyectil::Proyectil(sf::Texture& texturaProyectil,sf::Vector2f posInicial, sf::Vector2f dir, float alc, float vel, float danio, int id) {    
    velocidad = vel;
    alcanceMax = alc;
    this->danio = danio;
    distanciaRecorrida = 0.f;
    direccion.x = dir.x - posInicial.x;
    direccion.y = dir.y - posInicial.y;
    
    float longitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
    direccion.x /= longitud;
    direccion.y /= longitud;
    
    setHitbox(5.f, 5.f); // Hitbox pequeña para el proyectil
    
    setAngulo(std::atan2(direccion.y, direccion.x) * 180.f / 3.14159f);
    
    if(id == 0 ) {

        setHitbox(40,60);

    } else if (id == 7) {

        setHitbox(80,80);

    } else {

        sprite.setTexture(texturaProyectil);
        escalarSprite(1.5f, 1.5f);

    }
    setPosicionCentrado(posInicial.x, posInicial.y);

    estadoActivo = true;
}

bool Proyectil::debeDestruirse() const {
    return distanciaRecorrida >= alcanceMax || !estadoActivo;
}

void Proyectil::actualizar(float deltaTime,const std::vector<ObjetoMapa>& obstaculos) {

    // movimiento
    float desplazamiento = velocidad * deltaTime;
    mover(direccion.x * desplazamiento, direccion.y * desplazamiento);
    distanciaRecorrida += desplazamiento;

    // deteccion de obstaculos
    for(auto& obstaculo : obstaculos) {
        if (getHitbox().intersects(obstaculo.getHitbox())) {
            estadoActivo = false;
            break;
        }
    }
}

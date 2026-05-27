// Entidad.cpp - Clase base para todas las entidades

#include "Entidad.h"
#include "Mapa.h"

Entidad::Entidad(sf::Texture& textura)
    : posicion(0.f, 0.f),
      velocidad(0.f),
      salud(100),
      saludMaxima(100),
      vivo(true),
      mapa(nullptr)
{
    sprite.setTexture(textura);
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

Entidad::~Entidad() {
}

bool Entidad::esPosicionValida(sf::Vector2f nuevaPos) const {
    if (!mapa) return true;
    
    // Verificar las 4 esquinas del sprite aproximado
    float radio = 20.f;
    return !mapa->esBloqueante(nuevaPos + sf::Vector2f(-radio, -radio)) &&
           !mapa->esBloqueante(nuevaPos + sf::Vector2f(radio, -radio)) &&
           !mapa->esBloqueante(nuevaPos + sf::Vector2f(-radio, radio)) &&
           !mapa->esBloqueante(nuevaPos + sf::Vector2f(radio, radio));
}

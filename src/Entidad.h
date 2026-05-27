#pragma once

#include <SFML/Graphics.hpp>

// Forward declaration
class Mapa;

class Entidad {
public:
    Entidad(sf::Texture& textura);
    virtual ~Entidad();

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& ventana) = 0;

    sf::Vector2f getPosicion() const { return posicion; }
    void setPosicion(sf::Vector2f pos) { posicion = pos; sprite.setPosition(posicion); }
    bool estaVivo() const { return vivo; }
    virtual void recibirDano(int cantidad) { salud -= cantidad; if (salud <= 0) { salud = 0; vivo = false; } }
    int getSalud() const { return salud; }
    int getSaludMaxima() const { return saludMaxima; }
    sf::FloatRect getLimites() const { return sprite.getGlobalBounds(); }

    // Establecer el mapa para detección de colisión
    void setMapa(Mapa* mapa) { this->mapa = mapa; }

    // Verificar si una posición es válida (no choca con paredes)
    bool esPosicionValida(sf::Vector2f nuevaPos) const;

protected:
    sf::Sprite sprite;
    sf::Vector2f posicion;
    float velocidad;
    int salud;
    int saludMaxima;
    bool vivo;
    Mapa* mapa;
};

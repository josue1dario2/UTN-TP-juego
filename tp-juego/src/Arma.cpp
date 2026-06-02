#include "Arma.h"
#include <cmath>
#include <iostream>

Arma::Arma() {
    idArma = 1;
    nombre = "Pistola de Supervivencia";
    danio = 35.f;
    alcance = 600.f;
    cadencia = 0.5f; // 0.5 segundos entre disparos
    tiempoDesdeUltimoDisparo = 0.f;

    cargarTextura("assets/pistola.png");
    centrarOrigen();

    sf::FloatRect bounds = sprite.getLocalBounds();

    sprite.setOrigin(0.f, bounds.height / 2.f);

    escalarSprite(2.f, 2.f);
}

Arma::Arma(int id, std::string nom, int dmg, float alc, float cost) {
    idArma = id;
    nombre = nom;
    danio = dmg;
    alcance = alc;
}

float Arma::getDanio() const {
    return danio;
}

void Arma::actualizar(float deltaTime,const sf::Vector2f &posicionMouse, const sf::Vector2f &posicionJugador
    , std::vector<Proyectil>& proyectiles, sf::Texture& texturaProyectil) {
    // Lógica de actualización del arma, como animaciones o efectos de disparo
    tiempoDesdeUltimoDisparo += deltaTime;
    // Actualizar el ángulo de la mira
        float deltaX = posicionMouse.x - posicionJugador.x;
        float deltaY =  posicionJugador.y - posicionMouse.y;
        
        if (deltaX < 0) {
            escalarSprite(2.f, -2.f); // Voltear horizontalmente
            setPosicionCentrado(posicionJugador.x - 16, posicionJugador.y + 12.f);
        } else {
            escalarSprite(2.f, 2.f); // Escala normal
            setPosicionCentrado(posicionJugador.x + 16, posicionJugador.y + 12.f);
        }
        
        setAngulo(std::atan2(deltaY, deltaX) * -180.f / 3.14159f);


        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && tiempoDesdeUltimoDisparo >= cadencia) {
            proyectiles.emplace_back(texturaProyectil, getPosicion(), posicionMouse, 1000.f, 1500.f, getDanio());
            tiempoDesdeUltimoDisparo = 0.f; // Reiniciar el temporizador
        }
}

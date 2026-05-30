#include "Arma.h"
#include <cmath>
#include <iostream>

Arma::Arma() {
    idArma = 1;
    nombre = "Pistola de Supervivencia";
    danio = 35;
    alcance = 600.f;
    costo = 0.f;
    cargarTextura("assets/pistola.png");
    centrarOrigen();
    escalarSprite(2.f, 2.f);
}

Arma::Arma(int id, std::string nom, int dmg, float alc, float cost) {
    idArma = id;
    nombre = nom;
    danio = dmg;
    alcance = alc;
    costo = cost;
}

void Arma::actualizar(float deltaTime,const sf::Vector2f &posicionMouse, const sf::Vector2f &posicionJugador, std::vector<Proyectil>& proyectiles) {
    // Lógica de actualización del arma, como animaciones o efectos de disparo
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


        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            proyectiles.emplace_back(getPosicion(), posicionMouse, 1000.f, 500.f);
        }
}

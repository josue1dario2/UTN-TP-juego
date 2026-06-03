#include "Arma.h"
#include <cmath>
#include <iostream>

Arma::Arma() {
    idArma = -1;
}

Arma::Arma(int id, std::string nombre, float cadencia, float danio, float alcance, float costo, int municionMaxima, int tamanioCargador) {
    idArma = id;
    this->nombre = nombre;
    this->cadencia = cadencia;
    this->danio = danio;
    this->alcance = alcance;
    this->costo = costo;
    this->municionMaxima = municionMaxima;
    this->tamanioCargador = tamanioCargador;

    this->nombre = "assets/" + nombre + ".png";
    cargarTextura(this->nombre);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(0.f, bounds.height / 2.f);

    tiempoDesdeUltimoDisparo = 0.f;
}

float Arma::getDanio() const {
    return danio;
}
float Arma::getAlcance() const {
    return alcance;
}
int Arma::getIdArma() const {
    return idArma;
}

void Arma::actualizar(float deltaTime,const sf::Vector2f &posicionMouse, const sf::Vector2f &posicionJugador,
    std::vector<Proyectil>& proyectiles, sf::Texture& texturaProyectil) {
    // Lógica de actualización del arma, como animaciones o efectos de disparo
    tiempoDesdeUltimoDisparo += deltaTime;
    // Actualizar el ángulo de la mira
        float deltaX = posicionMouse.x - posicionJugador.x;
        float deltaY =  posicionJugador.y - posicionMouse.y;
        
        if (deltaX < 0) {
            escalarSprite(2.f, -2.f); // Voltear horizontalmente
            setPosicionCentrado(posicionJugador.x - 13, posicionJugador.y + 12.f);
        } else {
            escalarSprite(2.f, 2.f); // Escala normal
            setPosicionCentrado(posicionJugador.x + 13, posicionJugador.y + 12.f);
        }
        
        setAngulo(std::atan2(deltaY, deltaX) * -180.f / 3.14159f);

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && tiempoDesdeUltimoDisparo >= cadencia) {
            switch(getIdArma()) {

                case 2:
                { // Escopeta
                    disparoEscopeta(deltaX, deltaY, proyectiles, texturaProyectil);
                    break;
                }

                default:
                {
                    proyectiles.emplace_back(texturaProyectil, getPosicion(), posicionMouse, getAlcance(), 2000.f, getDanio());
                    break;
                }
            }
        tiempoDesdeUltimoDisparo = 0.f;
        }
}

void Arma::disparoEscopeta(float deltaX, float deltaY, std::vector<Proyectil>& proyectiles, sf::Texture& texturaProyectil) {
    const int cantidadPerdigones = 8;

    float anguloBase = std::atan2(deltaY, deltaX);

    for(int i = 0; i < cantidadPerdigones; i++) {
        // Desviación aleatoria entre -10° y +10°
        float desviacionGrados = (std::rand() % 21) - 10;

        float desviacionRadianes = desviacionGrados * 3.14159f / 180.f;

        float anguloFinal = anguloBase + desviacionRadianes;

        sf::Vector2f objetivo;

        objetivo.x = getPosicion().x + std::cos(anguloFinal) * 1000.f;

        objetivo.y = getPosicion().y - std::sin(anguloFinal) * 1000.f;

        proyectiles.emplace_back(texturaProyectil, getPosicion(), objetivo, getAlcance(), 2000.f, getDanio());
    }
}

bool Arma::estaDisponible() const {
    return idArma != -1; // Un id de -1 indica un arma no disponible
}
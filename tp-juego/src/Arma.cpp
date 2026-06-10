#include "../include/Arma.h"
#include <cmath>
#include <iostream>

Arma::Arma() {
    idArma = -1;
    desbloqueada = false;
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

    std::string rutaTextura = "assets/" + nombre + ".png";
    this->nombre = nombre;
    cargarTextura(rutaTextura);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(0.f, bounds.height / 2.f);

    tiempoDesdeUltimoDisparo = 0.f;
    tiempoRecarga = 0.f;

    switch(id) {

        case 0: { //cuchillo
            municionActual = 0;
            municionEnCargador = 2;
            desbloqueada = true;
            break;
        }

        default: {
            municionActual = municionMaxima/2; // Empieza con la mitad de la munición total
            municionEnCargador = 0;
            desbloqueada = false;
            break;
        }
    }

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

std::string Arma::getNombre() const {
    return nombre;
}

void Arma::actualizar(float deltaTime,const sf::Vector2f &posicionMouse, const sf::Vector2f &posicionJugador,
    std::vector<Proyectil>& proyectiles, sf::Texture& texturaProyectil) {
    // Lógica de actualización del arma, como animaciones o efectos de disparo
    tiempoDesdeUltimoDisparo += deltaTime;
    tiempoRecarga += deltaTime;
    // ----------------- Actualizar el ángulo de la mira
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

        // ----------------- Lógica de disparo
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && tiempoDesdeUltimoDisparo >= cadencia && municionEnCargador > 0 && !enRecarga) {
            // el switch es para poder manejar los disparos especiales
            switch(getIdArma()) { 

                case 0: { //cuchillo
                    proyectiles.emplace_back(texturaProyectil, getPosicion(), posicionMouse, getAlcance(), 500.f, getDanio());
                    municionEnCargador = 2;
                    break;

                }

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


            std::cout << "Disparando " << getNombre() << ". Munición en cargador antes de disparar: " << municionEnCargador << std::endl;
            municionEnCargador--;
            std::cout << "Munición en cargador después de disparar: " << municionEnCargador << std::endl;
            tiempoDesdeUltimoDisparo = 0.f;
        }

        // ----------------- Lógica de recarga
        if (tiempoRecarga >= 1.f) {
            enRecarga = false;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R) && municionEnCargador < tamanioCargador && !enRecarga && municionActual > 0) {
            int cantidad = 0;

            if (municionActual >= (tamanioCargador - municionEnCargador)) {
                cantidad = tamanioCargador - municionEnCargador;
            } else {
                cantidad = municionActual;
            }
            municionActual -= cantidad;

            recargar(cantidad);
            tiempoRecarga = 0.f;
            enRecarga = true;

            std::cout << "Recarga ejecutada con " << getNombre() << ". Munición en restante: " << municionActual << std::endl;
        }
}

void Arma::recargar(int cantidad) {
    municionEnCargador += cantidad;
}

bool Arma::estaDisponible() const {
    return desbloqueada; // Un id de -1 indica un arma no disponible
}

void Arma::setDesbloqueo(bool estado) {
    desbloqueada = estado;
}

// llena la municion actual
void Arma::llenarMunicion() {
    municionActual = municionMaxima;
}

// ------------------ Lógica de disparos especiales
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
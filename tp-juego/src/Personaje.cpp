#include "Personaje.h"

Personaje::Personaje() {
    // Valores por defecto por ahora
    idPersonaje = 0;
    nombre = "Superviviente";
    vidaMax = 100.f;
    vidaActual = 100.f;
    armaduraMax = 50.f;
    armaduraActual = 50.f;
    velocidad = 200.f;
    habilidad = "Ninguna";


    cargarTextura("assets/jugador2.png");

    setearTamanioSprite(13, 16);
    centrarOrigen(); //centrar el origen del sprite para facilitar el posicionamiento
    sprite.setScale(3.f,3.f);

    setHitbox(13.f * 2.f, 16.f * 2.1f); // Ajustar el tamaño del hitbox según el sprite escalado

    setPosicion(1920.f, 1080.f);
}

void Personaje::cargarAtributos(int id, std::string nom, float vida, float armadura, float vel, std::string hab) {
    idPersonaje = id;
    nombre = nom;
    vidaMax = vida;
    vidaActual = vida;
    armaduraMax = armadura;
    armaduraActual = armadura;
    velocidad = vel;
    habilidad = hab;
}

void Personaje::controlar(float movimiento) {
    // Movimiento personaje
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        mover(0.f, -movimiento);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        mover(0.f, movimiento);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        mover(-movimiento, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        mover(movimiento, 0.f);
    }
}

void Personaje::actualizar(float deltaTime) {
    // lógica adicional para el personaje, como animaciones o habilidades
    float movimiento = velocidad * deltaTime;
    controlar(movimiento);

}
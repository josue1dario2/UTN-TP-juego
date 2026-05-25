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
    centrarOrigen();

    sprite.setScale(3.f,3.f);
    sprite.setPosition(1920.f, 1080.f);
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
        //cuerpo.move(0.f, -velocidad);
        sprite.move(0.f, -movimiento);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        //cuerpo.move(0.f, velocidad);
        sprite.move(0.f, movimiento);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        //cuerpo.move(-velocidad, 0.f);
        sprite.move(-movimiento, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        //cuerpo.move(velocidad, 0.f);
        sprite.move(movimiento, 0.f);
    }
}

void Personaje::actualizar(float deltaTime) {
    // lógica adicional para el personaje, como animaciones o habilidades
    float movimiento = velocidad * deltaTime;
    controlar(movimiento);

}
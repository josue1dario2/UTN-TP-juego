#include "Personaje.h"

Personaje::Personaje() {

    cuerpo.setSize(sf::Vector2f(40.f, 40.f)); 
    cuerpo.setFillColor(sf::Color::Green); 
    cuerpo.setOrigin(20.f, 20.f);          // Centro del personaje
    cuerpo.setPosition(400.f, 300.f);      // Centro de la pantalla

    // Valores por defecto por ahora
    idPersonaje = 0;
    nombre = "Superviviente";
    vidaMax = 100.f;
    vidaActual = 100.f;
    armaduraMax = 50.f;
    armaduraActual = 50.f;
    velocidad = 4.f;
    habilidad = "Ninguna";

    textJug.loadFromFile("assets/jugador2.png");
    spriteJug.setTexture(textJug);
    spriteJug.setTextureRect(sf::IntRect(0,0,13,16));
    spriteJug.setScale(3.f,3.f);
    spriteJug.setOrigin(6.5f, 8.f); // Centro del sprite
    spriteJug.setPosition(400.f, 300.f);
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

void Personaje::controlar() {
    // Movimiento personaje
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        cuerpo.move(0.f, -velocidad);
        spriteJug.move(0.f, -velocidad);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        cuerpo.move(0.f, velocidad);
        spriteJug.move(0.f, velocidad);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        cuerpo.move(-velocidad, 0.f);
        spriteJug.move(-velocidad, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        cuerpo.move(velocidad, 0.f);
        spriteJug.move(velocidad, 0.f);
    }
}

void Personaje::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(cuerpo);
    ventana.draw(spriteJug);
}
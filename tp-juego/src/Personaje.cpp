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
    mapaColision = nullptr;

    cargarTextura("assets/jugador2.png");

    setearTamanioSprite(13, 16);
    centrarOrigen(); // centrar el origen del sprite para facilitar el posicionamiento
    escalarSprite(3.f, 3.f);

    setHitbox(13.f * 2.f, 16.f * 2.1f); // Ajustar el tamaño del hitbox según el sprite escalado

    setPosicionCentrado(1720.f, 1080.f);

    armaEquipada = Arma(1, "Pistola de Supervivencia", 35, 600.f, 0.f);

    movimientoX = 0.f;
    movimientoY = 0.f;
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

void Personaje::guardarPosicionAnterior() {
    posicionAnterior = getPosicion();
}

void Personaje::volverPosicionAnteriorX() {
    setPosicionCentrado(posicionAnterior.x, getPosicion().y);
}

void Personaje::volverPosicionAnteriorY() {
    setPosicionCentrado(getPosicion().x, posicionAnterior.y);
}

void Personaje::actualizar(float deltaTime) {
    // lógica adicional para el personaje, como animaciones o habilidades
    movimientoX = 0.f;
    movimientoY = 0.f;

    float movimiento = velocidad * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movimientoX -= movimiento;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movimientoX += movimiento;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movimientoY -= movimiento;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movimientoY += movimiento;
    }
}

float Personaje::getMovimientoX() const {
    return movimientoX;
}

float Personaje::getMovimientoY() const {
    return movimientoY;
}
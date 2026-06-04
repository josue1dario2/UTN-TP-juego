#include "Personaje.h"
#include <cmath>

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
    centrarOrigen(); // centrar el origen del sprite para facilitar el posicionamiento
    escalarSprite(3.f, 3.f);

    setHitbox(13.f * 2.f, 16.f * 2.1f); // Ajustar el tamaño del hitbox según el sprite escalado

    setPosicionCentrado(1720.f, 1080.f);

    inventarioArmas.emplace_back();

    armaEquipada = 0;

    movimientoX = 0.f;
    movimientoY = 0.f;
}


void Personaje::seleccionarPersonaje(int tipo) {
    if (tipo == 0) {
        cargarAtributos(0, "Superviviente", 100.f, 50.f, 200.f, "Equilibrado");
        cargarTextura("assets/jugador2.png");
    }
    else if (tipo == 1) {
        cargarAtributos(1, "Tanque", 160.f, 100.f, 140.f, "Resistencia");
        cargarTextura("assets/jugador2.png");
    }
    else if (tipo == 2) {
        cargarAtributos(2, "Explorador", 80.f, 25.f, 280.f, "Velocidad");
        cargarTextura("assets/jugador2.png");
    }

    setearTamanioSprite(13, 16);
    centrarOrigen();
    escalarSprite(3.f, 3.f);
    setHitbox(13.f * 2.f, 16.f * 2.1f);
    setPosicionCentrado(1720.f, 1080.f);
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

void Personaje::actualizar(float deltaTime, const std::vector<ObjetoMapa>& obstaculos) {
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

    //movimiento horizontal jugador, chequeo de colisiones mediante bucle for
    guardarPosicionAnterior();
    mover(getMovimientoX(), 0.f);
    for(auto& obstaculo : obstaculos) {
        if (getHitbox().intersects(obstaculo.getHitbox())) {
            volverPosicionAnteriorX();
            break;
        }
    }

    //movimiento vertical jugador
    guardarPosicionAnterior();
    mover(0.f, getMovimientoY());
    for(auto& obstaculo : obstaculos) {
        if (getHitbox().intersects(obstaculo.getHitbox())) {
            volverPosicionAnteriorY();
            break;
        }
    }
}

float Personaje::getMovimientoX() const {
    return movimientoX;
}

float Personaje::getMovimientoY() const {
    return movimientoY;
}

Arma& Personaje::getArma() {
    return inventarioArmas[armaEquipada];
}
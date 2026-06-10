#include "../include/Personaje.h"
#include <cmath>

Personaje::Personaje(){
    idPersonaje = 0;
    nombre = "Superviviente";
    vidaMax = 1.f;
    vidaActual = 1.f;
    armaduraMax = 1.f;
    armaduraActual = 1.f;
    velocidad = 1.f;
    habilidad = "-";
}

Personaje::Personaje(int id, int idArmaEspecial, std::string nombre, float vida, float armadura, float velocidad, float cooldownHabilidad) {
    idPersonaje = id;
    this->nombre = nombre;
    vidaMax = vida;
    vidaActual = vida;
    armaduraMax = armadura;
    armaduraActual = armadura;
    this->velocidad = velocidad;
    this->cooldownHabilidad = cooldownHabilidad;
    habilidad = "-";
    armaEquipada = 0;

    mostrarHitbox = false;
    cargarTextura("assets/" + nombre + ".png");
    escalarSprite(0.8f,0.8f);
    centrarOrigen();

    setHitbox(13.f * 2.f, 16.f * 2.1f);
    setPosicionCentrado(1720.f, 1080.f);

    archivoArma archivo("armas.dat");

    inventarioArmas.reserve(5);
    archivo.entregarArma(inventarioArmas, 0);
    archivo.entregarArma(inventarioArmas, 1);
    archivo.entregarArma(inventarioArmas, 2);
    archivo.entregarArma(inventarioArmas, 3);
    archivo.entregarArma(inventarioArmas, idArmaEspecial);

    inventarioArmas[0].setDesbloqueo(true);
    inventarioArmas[1].setDesbloqueo(true);
    inventarioArmas[2].setDesbloqueo(true);
    inventarioArmas[3].setDesbloqueo(true);
}

void Personaje::guardarPosicionAnterior() { posicionAnterior = getPosicion(); }

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

    // movimiento horizontal jugador, chequeo de colisiones mediante bucle for
    guardarPosicionAnterior();
    mover(getMovimientoX(), 0.f);
    for(auto& obstaculo : obstaculos) {
        if (getHitbox().intersects(obstaculo.getHitbox())) {
            volverPosicionAnteriorX();
            break;
        }
    }

    // movimiento vertical jugador
    guardarPosicionAnterior();
    mover(0.f, getMovimientoY());
    for(auto& obstaculo : obstaculos) {
        if (getHitbox().intersects(obstaculo.getHitbox())) {
            volverPosicionAnteriorY();
            break;
        }
    }

    elegirArma();
}

float Personaje::getMovimientoX() const { return movimientoX; }

float Personaje::getMovimientoY() const { return movimientoY; }

// devuelve el arma así se actualiza en la clase juego
Arma& Personaje::getArma() {
    return inventarioArmas[armaEquipada];
}

void Personaje::elegirArma() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        armaEquipada = 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && inventarioArmas[1].estaDisponible()) {
        armaEquipada = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && inventarioArmas[2].estaDisponible()) {
        armaEquipada = 2;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && inventarioArmas[3].estaDisponible()) {
        armaEquipada = 3;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) && inventarioArmas[4].estaDisponible()) {
        armaEquipada = 4;
    }
}

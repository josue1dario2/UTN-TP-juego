#include "../include/Personaje.h"
#include <cmath>

/*
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
    
    cargarTextura("assets/jugador.png");
    
    setearTamanioSprite(39, 48);
    centrarOrigen(); // centrar el origen del sprite para facilitar el posicionamiento
    //escalarSprite(3.f, 3.f);
    
    setHitbox(13.f * 2.f, 16.f * 2.1f); // Ajustar el tamaño del hitbox según el sprite escalado
    
    setPosicionCentrado(1720.f, 1080.f);
    
    inventarioArmas.reserve(5);
    
    archivoArma archivo("armas.dat");
    
    archivo.entregarArma(inventarioArmas, 1);
    archivo.entregarArma(inventarioArmas, 0);
    archivo.entregarArma(inventarioArmas, 2);
    archivo.entregarArma(inventarioArmas, 3);
    archivo.entregarArma(inventarioArmas, 4);
    
    armaEquipada = 0;
    
    movimientoX = 0.f;
    movimientoY = 0.f;
}
*/

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

Personaje::Personaje(int id, int idArmaEspecial, std::string nombre, float vida, float armadura, float velocidad) {
    idPersonaje = id;
    this->nombre = nombre;
    vidaMax = vida;
    vidaActual = vida;
    armaduraMax = armadura;
    armaduraActual = armadura;
    this->velocidad = velocidad;
    habilidad = "-";

    cargarTextura("assets/" + nombre + ".png");
    setearTamanioSprite(39, 48);
    centrarOrigen();

    setHitbox(13.f * 2.f, 16.f * 2.1f);
    setPosicionCentrado(1720.f, 1080.f);

    archivoArma archivo("armas.dat");

    inventarioArmas.reserve(5);
    archivo.entregarArma(inventarioArmas, 1);
    archivo.entregarArma(inventarioArmas, 0);
    archivo.entregarArma(inventarioArmas, 2);
    archivo.entregarArma(inventarioArmas, 3);
    archivo.entregarArma(inventarioArmas, 4);
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

    elegirArma();
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
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
    habilidadActivada = false;
    armaEquipada = 0;
}

Personaje::Personaje(int id, int idArmaEspecial, std::string nombre, float vida, float armadura, float velocidad, float cooldownHabilidad) {
    idPersonaje = id;
    this->nombre = nombre;
    vidaMax = vida;
    vidaActual = vida;
    armaduraMax = armadura;
    armaduraActual = armadura;
    this->velocidad = velocidad;
    if (this->velocidad <= 0.f) this->velocidad = 200.f;
    this->cooldownHabilidad = cooldownHabilidad;
    habilidad = "-";
    armaEquipada = 0;
    habilidadActivada = false;
    tiempoHabilidad = 0;
    habilidadDisponible = true;
    multiplicadorZoom = 1.f;

    mostrarHitbox = false;
    if (!cargarTextura("assets/" + nombre + ".png")) {
        cargarTextura("assets/jugador.png");
    }
    escalarSprite(0.8f,0.8f);
    //setearTamanioSprite(39, 48);
    centrarOrigen();

    setHitbox(13.f * 2.f, 16.f * 2.1f);
    setPosicionCentrado(1720.f, 1080.f);
    posicionAnterior = sf::Vector2f(1720.f, 1080.f);

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

void Personaje::actualizar(float deltaTime, const std::vector<ObjetoMapa>& obstaculos, const std::vector<sf::FloatRect>& obstaculosAdicionales) {
    // logica adicional para el personaje, como animaciones o habilidades
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
    bool colisionoX = false;
    for(auto& obstaculo : obstaculos) {
        if (getHitbox().intersects(obstaculo.getHitbox())) {
            volverPosicionAnteriorX();
            colisionoX = true;
            break;
        }
    }
    if (!colisionoX) {
        for(const auto& rect : obstaculosAdicionales) {
            if (getHitbox().intersects(rect)) {
                volverPosicionAnteriorX();
                break;
            }
        }
    }

    // movimiento vertical jugador
    guardarPosicionAnterior();
    mover(0.f, getMovimientoY());
    bool colisionoY = false;
    for(auto& obstaculo : obstaculos) {
        if (getHitbox().intersects(obstaculo.getHitbox())) {
            volverPosicionAnteriorY();
            colisionoY = true;
            break;
        }
    }
    if (!colisionoY) {
        for(const auto& rect : obstaculosAdicionales) {
            if (getHitbox().intersects(rect)) {
                volverPosicionAnteriorY();
                break;
            }
        }
    }

    elegirArma();

    tiempoHabilidad += deltaTime;

    activarHabilidad(deltaTime);
}

float Personaje::getMovimientoX() const { return movimientoX; }

float Personaje::getMovimientoY() const { return movimientoY; }

// devuelve el arma así se actualiza en la clase juego
Arma& Personaje::getArma() {
    return inventarioArmas[armaEquipada];
}

void Personaje::setVelocidad(float velocidad){
    this->velocidad = velocidad;
}

float Personaje::getMultiplicadorZoom(){
    return multiplicadorZoom;
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

void Personaje::activarHabilidad(float deltaTime){
    
    if (tiempoHabilidad >= cooldownHabilidad && habilidadActivada == false){
        habilidadDisponible = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && habilidadDisponible == true && habilidadActivada == false){
        habilidadActivada = true;
        tiempoHabilidad = 0;
        habilidadDisponible = false;
        std::cout << "se activo la habilidad" << std::endl;
    }
    
    switch (idPersonaje) {

        case 0: {
            habilidadRecon(deltaTime);
            break;
        }

    }
}

void Personaje::habilidadRecon(float deltaTime) {

    if(habilidadActivada) {
    
        setVelocidad(0);

        if(multiplicadorZoom < 2.f){
            multiplicadorZoom += deltaTime;
        }

        if (tiempoHabilidad >= 10){
            std::cout << "habilidad desactivada" << std:: endl;
            habilidadActivada = false;
            setVelocidad(200);
            tiempoHabilidad = 0;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && tiempoHabilidad > 1){
            std::cout << "habilidad desactivada" << std:: endl;
            habilidadActivada = false;
            setVelocidad(200);
            tiempoHabilidad = 0;
        }
        
    } else {
        if(multiplicadorZoom > 1.f){
            multiplicadorZoom -= deltaTime;
        }
    }
}

void Personaje::recibirDanio(float cantidad) {
    if (armaduraActual > 0) {
        float absorcion = cantidad * 0.5f;
        if (armaduraActual >= absorcion) {
            armaduraActual -= absorcion;
            vidaActual -= (cantidad - absorcion);
        } else {
            float remanente = absorcion - armaduraActual;
            armaduraActual = 0;
            vidaActual -= (cantidad - absorcion + remanente);
        }
    } else {
        vidaActual -= cantidad;
    }

    if (vidaActual < 0) {
        vidaActual = 0;
    }
}

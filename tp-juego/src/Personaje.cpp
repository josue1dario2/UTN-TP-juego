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
    velocidadNormal = velocidad;
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
    inventarioArmas[4].setDesbloqueo(true);

    zonaHabilidad = sf::FloatRect(0 , 0, 0, 0);
}

void Personaje::guardarPosicionAnterior() { posicionAnterior = getPosicion(); }

void Personaje::volverPosicionAnteriorX() {
  setPosicionCentrado(posicionAnterior.x, getPosicion().y);
}

void Personaje::volverPosicionAnteriorY() {
  setPosicionCentrado(getPosicion().x, posicionAnterior.y);
}

void Personaje::actualizar(float deltaTime, const std::vector<ObjetoMapa>& obstaculos, const std::vector<sf::FloatRect> &hitboxZombies, const sf::Vector2f &posicionMouse) {
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
        for(const auto& rect : hitboxZombies) {
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
        for(const auto& rect : hitboxZombies) {
            if (getHitbox().intersects(rect)) {
                volverPosicionAnteriorY();
                break;
            }
        }
    }
    
    elegirArma();

    calcularDireccion(posicionMouse);

    actualizarZonaHabilidad();
    
    tiempoHabilidad += deltaTime;
    activarHabilidad(deltaTime);
    
}

void Personaje::actualizarZonaHabilidad(){
    //zona habilidad
    switch (direccion){
        case 1: {
            //izquierda
            zonaHabilidad = sf::FloatRect(getPosicion().x - 75.f, getPosicion().y - 50.f, 50, 100);
            break;
        }
        case 2: {
            //arriba
            zonaHabilidad = sf::FloatRect(getPosicion().x - 50.f, getPosicion().y - 75.f, 100, 50);
            break;
        }
        case 3: {
            //derecha
            zonaHabilidad = sf::FloatRect(getPosicion().x + 25.f, getPosicion().y - 50.f, 50, 100);
            break;
        }
        case 4: {
            //abajo
            zonaHabilidad = sf::FloatRect(getPosicion().x - 50.f, getPosicion().y + 25.f, 100, 50);
            break;
        }
    }
}

void Personaje::calcularDireccion(sf::Vector2f posicionMouse) {
    float dx = posicionMouse.x - getPosicion().x;
    float dy = posicionMouse.y - getPosicion().y;
    
    if (std::abs(dx) > std::abs(dy)) {
        if(dx > 0) {
            direccion = 3; // derecha
        }
        else {
            direccion = 1;  // izquierda
        }
    }
    else {
        if(dy > 0) {
            direccion = 4; // abajo
        }
        else {
            direccion = 2;  // arriba
        }
    }
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

// --------------------- HABILIDADES -------------------
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
        case 1: {
            habilidadJoel(deltaTime);
            break;
        }
        case 2: {
            habilidadGhost(deltaTime);
            break;
        }
    }
}

void Personaje::habilidadGhost(float deltaTime) {

    
    if(habilidadActivada) {
        if(velocidad == velocidadNormal){
            setVelocidad(getVelocidad()*2);
        }
        
        if (tiempoHabilidad >= 8) {
            habilidadActivada = false;
            setVelocidad(velocidadNormal);
            tiempoHabilidad = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && tiempoHabilidad > 1){
            habilidadActivada = false;
            setVelocidad(velocidadNormal);
            tiempoHabilidad = 0;
        }
    }

}

void Personaje::habilidadJoel(float deltaTime){
    if (habilidadActivada){
        if(tiempoHabilidad >= 0.1f){
            std::cout << "habilidad desactivada" << std:: endl;
            habilidadActivada = false;
            tiempoHabilidad = 0;
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

// --------------- GETTERS SETTERS ----------------
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

sf::FloatRect Personaje::getZonaHabilidad() const{
    return zonaHabilidad;
}

bool Personaje::habilidadActiva() const{
    return habilidadActivada;
}

int Personaje::getDireccion() const{
    return direccion;
}
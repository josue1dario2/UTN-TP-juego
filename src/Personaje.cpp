#include "Personaje.h"

Personaje::Personaje()
    : id(0), nombre(""), vidaInicial(0), armaduraInicial(0), 
      velocidadInicial(0), habilidad("") {
}

Personaje::Personaje(int id, const std::string& nombre, int vida, int armadura,
                    int velocidad, const std::string& habilidad)
    : id(id), nombre(nombre), vidaInicial(vida), armaduraInicial(armadura),
      velocidadInicial(velocidad), habilidad(habilidad) {
}

int Personaje::getId() const {
    return id;
}

std::string Personaje::getNombre() const {
    return nombre;
}

int Personaje::getVidaInicial() const {
    return vidaInicial;
}

int Personaje::getArmaduraInicial() const {
    return armaduraInicial;
}

int Personaje::getVelocidadInicial() const {
    return velocidadInicial;
}

std::string Personaje::getHabilidad() const {
    return habilidad;
}

void Personaje::setId(int valor) {
    id = valor;
}

void Personaje::setNombre(const std::string& valor) {
    nombre = valor;
}

void Personaje::setVidaInicial(int valor) {
    vidaInicial = valor;
}

void Personaje::setArmaduraInicial(int valor) {
    armaduraInicial = valor;
}

void Personaje::setVelocidadInicial(int valor) {
    velocidadInicial = valor;
}

void Personaje::setHabilidad(const std::string& valor) {
    habilidad = valor;
}
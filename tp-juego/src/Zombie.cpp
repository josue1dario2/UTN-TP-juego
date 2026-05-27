#include "Zombie.h"
#include <iostream>

Zombie::Zombie() : Entidad() {
    tipo = 1;
    vida = 100;
    ataque = 10;
    velocidad = 1.0f;
    
    // Inicializar campos de Entidad
    vidaMax = 100.0f;
    vidaActual = 100.0f;
    this->velocidad = 1.0f; // Campo de Entidad
}

Zombie::Zombie(int tipo, int vida, int ataque, float velocidad) : Entidad() {
    this->tipo = tipo;
    this->vida = vida;
    this->ataque = ataque;
    this->velocidad = velocidad;

    // Sincronizar campos de Entidad
    vidaMax = static_cast<float>(vida);
    vidaActual = static_cast<float>(vida);
    this->velocidad = velocidad;
}

int Zombie::getTipo() const {
    return tipo;
}

void Zombie::setTipo(int tipo) {
    this->tipo = tipo;
}

int Zombie::getVida() const {
    return vida;
}

void Zombie::setVida(int vida) {
    this->vida = vida;
    vidaActual = static_cast<float>(vida);
}

int Zombie::getAtaque() const {
    return ataque;
}

void Zombie::setAtaque(int ataque) {
    this->ataque = ataque;
}

float Zombie::getVelocidad() const {
    return velocidad;
}

void Zombie::setVelocidad(float velocidad) {
    this->velocidad = velocidad;
}

void Zombie::atacar() {
    // Implementación del ataque (por ejemplo, mensaje de depuración)
    std::cout << "Zombie tipo " << tipo << " atacando con " << ataque << " de danio." << std::endl;
}

void Zombie::quitarVida(int cantidad) {
    vida -= cantidad;
    if (vida < 0) {
        vida = 0;
    }
    vidaActual = static_cast<float>(vida);
}

bool Zombie::muerto() const {
    return vida <= 0;
}

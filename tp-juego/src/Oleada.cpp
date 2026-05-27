#include "Oleada.h"
#include <iostream>

Oleada::Oleada() {
    numero = 1;
    enemigosTotales = 0;
    enemigosVivos = 0;
    completada = false;
}

Oleada::Oleada(int numero, int enemigosTotales) {
    this->numero = numero;
    this->enemigosTotales = enemigosTotales;
    this->enemigosVivos = enemigosTotales;
    this->completada = false;
}

int Oleada::getNumero() const {
    return numero;
}

void Oleada::setNumero(int numero) {
    this->numero = numero;
}

int Oleada::getEnemigosTotales() const {
    return enemigosTotales;
}

void Oleada::setEnemigosTotales(int enemigosTotales) {
    this->enemigosTotales = enemigosTotales;
}

int Oleada::getEnemigosVivos() const {
    return enemigosVivos;
}

void Oleada::setEnemigosVivos(int enemigosVivos) {
    this->enemigosVivos = enemigosVivos;
}

bool Oleada::isCompletada() const {
    return completada;
}

void Oleada::setCompletada(bool completada) {
    this->completada = completada;
}

const std::vector<Zombie>& Oleada::getEnemigos() const {
    return enemigos;
}

std::vector<Zombie>& Oleada::getEnemigos() {
    return enemigos;
}

void Oleada::iniciar() {
    completada = false;
    enemigosVivos = enemigosTotales;
    enemigos.clear();

    // Crear la cantidad requerida de zombies para la oleada
    for (int i = 0; i < enemigosTotales; i++) {
        // Los atributos del zombie pueden escalar con el número de oleada
        int vidaBase = 50 + (numero * 10);
        int ataqueBase = 5 + (numero * 2);
        float velBase = 1.0f + (numero * 0.1f);
        
        // Creamos zombies de tipo simple (tipo 1)
        enemigos.push_back(Zombie(1, vidaBase, ataqueBase, velBase));
    }
    
    std::cout << "Oleada " << numero << " iniciada con " << enemigosTotales << " enemigos vivos." << std::endl;
}

void Oleada::actualizar() {
    if (completada) return;

    int vivos = 0;
    for (auto& zombie : enemigos) {
        if (!zombie.muerto()) {
            // Aquí se actualizaría la posición/física de cada zombie si fuera necesario
            vivos++;
        }
    }

    enemigosVivos = vivos;

    if (enemigosVivos == 0 && enemigosTotales > 0) {
        completada = true;
        std::cout << "Oleada " << numero << " completada." << std::endl;
    }
}

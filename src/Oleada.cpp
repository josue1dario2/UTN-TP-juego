// Oleada.cpp - Implementacion de Oleada segun diagrama UML

#include "Oleada.h"
#include "Zombie.h"
#include <iostream>
#include <cstdlib>

Oleada::Oleada()
    : numero(0), enemigosTotales(0), enemigosVivos(0), completada(false) {
}

Oleada::~Oleada() {
    for (auto* z : zombies) {
        delete z;
    }
}

// === Segun diagrama UML ===
void Oleada::iniciar(int numeroOleada) {
    numero = numeroOleada;
    enemigosTotales = 5 + numeroOleada * 3;
    enemigosVivos = enemigosTotales;
    completada = false;
    std::cout << "Oleada " << numero << " iniciada. Enemigos: " << enemigosTotales << std::endl;
}

void Oleada::actualizar() {
    if (completada) return;
    for (auto* z : zombies) {
        if (z->muerto()) {
            enemigosVivos--;
        }
    }
    if (enemigosVivos <= 0) {
        enemigosVivos = 0;
        completada = true;
        std::cout << "Oleada " << numero << " completada!" << std::endl;
    }
}

// === Compatibilidad con Juego.cpp ===
void Oleada::iniciarOleada(int numeroOleada) {
    numero = numeroOleada;
    enemigosTotales = 5 + numeroOleada * 3;
    enemigosVivos = enemigosTotales;
    completada = false;
}

void Oleada::actualizar(float dt) {
    if (completada) return;
    for (auto* z : zombies) {
        if (z->muerto()) {
            enemigosVivos--;
        }
    }
    if (enemigosVivos <= 0) {
        enemigosVivos = 0;
        completada = true;
    }
}

bool Oleada::oleadaCompletada() const {
    return completada;
}

bool Oleada::debeSpawnearZombie() {
    // Por implementar: controlar spawn rate
    return !completada && enemigosVivos > 0;
}

void Oleada::zombieMuerto() {
    enemigosVivos--;
    if (enemigosVivos <= 0) {
        enemigosVivos = 0;
        completada = true;
    }
}

int Oleada::getOleadaActual() const {
    return numero;
}

int Oleada::getZombiesRestantes() const {
    return enemigosVivos;
}

bool Oleada::estaOleadaActiva() const {
    return !completada;
}

TipoZombie Oleada::getZombieTypeForWave() const {
    int dado = std::rand() % 100;
    if (numero <= 3) {
        return (dado < 85) ? TipoZombie::NORMAL : TipoZombie::RAPIDO;
    }
    if (numero <= 6) {
        if (dado < 55) return TipoZombie::NORMAL;
        if (dado < 85) return TipoZombie::RAPIDO;
        return TipoZombie::PESADO;
    }
    if (dado < 35) return TipoZombie::NORMAL;
    if (dado < 65) return TipoZombie::RAPIDO;
    return TipoZombie::PESADO;
}

// === Getters UML ===
int Oleada::getNumero() const {
    return numero;
}

int Oleada::getEnemigosVivos() const {
    return enemigosVivos;
}

bool Oleada::estaCompletada() const {
    return completada;
}

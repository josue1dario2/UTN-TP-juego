// Oleada.cpp - Sistema de oleadas de zombies

#include "Oleada.h"
#include <cstdlib>
#include <algorithm>

// Constructor: inicializa todo en cero
Oleada::Oleada()
    : oleadaActual(0),
      totalZombies(0),
      zombiesSpawnados(0),
      zombiesMuertos(0),
      intervaloSpawn(2.0f),
      timerSpawn(0.f),
      oleadaActiva(false),
      tiempoEntreOleadas(5.0f),
      timerTransicion(0.f),
      enTransicion(false)
{
}

// inicia una nueva oleada
void Oleada::iniciarOleada(int numeroOleada) {
    oleadaActual = numeroOleada;

    // mas zombies por oleada: oleada 1 = 8, oleada 5 = 20, oleada 10 = 35
    totalZombies = 5 + numeroOleada * 3;

    zombiesSpawnados = 0;
    zombiesMuertos = 0;

    // spawning mas rapido en oleadas avanzadas (minimo 0.4s)
    intervaloSpawn = std::max(0.4f, 2.0f - numeroOleada * 0.15f);
    timerSpawn = 0.f;

    oleadaActiva = true;
    enTransicion = false;
}

// actualiza: actualiza timers cada frame
void Oleada::actualizar(float dt) {
    // --- Transicion: descanso entre oleadas ---
    if (enTransicion) {
        timerTransicion -= dt;
        if (timerTransicion <= 0.f) {
            enTransicion = false;
            iniciarOleada(oleadaActual + 1);
        }
        return;
    }

    // --- Oleada activa: spawning ---
    if (oleadaActiva) {
        if (timerSpawn > 0.f) {
            timerSpawn -= dt;
        }

        if (oleadaCompletada()) {
            oleadaActiva = false;
            enTransicion = true;
            timerTransicion = tiempoEntreOleadas;
        }
    }
}

// true si hay que spawnear un nuevo zombie
bool Oleada::debeSpawnearZombie() {
    if (oleadaActiva && timerSpawn <= 0.f && zombiesSpawnados < totalZombies) {
        timerSpawn = intervaloSpawn;
        zombiesSpawnados++;
        return true;
    }
    return false;
}

// notifica que un zombie fue eliminado
void Oleada::zombieMuerto() {
    zombiesMuertos++;
}

// true si la oleada termino (todos spawneados y todos muertos)
bool Oleada::oleadaCompletada() const {
    return zombiesSpawnados >= totalZombies && zombiesMuertos >= totalZombies;
}

// determina que tipo de zombie aparece segun la oleada
TipoZombie Oleada::getZombieTypeForWave() const {
    int dado = std::rand() % 100;

    if (oleadaActual <= 3) {
        return (dado < 85) ? TipoZombie::NORMAL : TipoZombie::RAPIDO;
    }

    if (oleadaActual <= 6) {
        if (dado < 55) return TipoZombie::NORMAL;
        if (dado < 85) return TipoZombie::RAPIDO;
        return TipoZombie::PESADO;
    }

    if (dado < 35) return TipoZombie::NORMAL;
    if (dado < 65) return TipoZombie::RAPIDO;
    return TipoZombie::PESADO;
}

// --- Getters ---

int Oleada::getOleadaActual() const {
    return oleadaActual;
}

int Oleada::getZombiesRestantes() const {
    return totalZombies - zombiesMuertos;
}

bool Oleada::estaOleadaActiva() const {
    return oleadaActiva;
}

bool Oleada::estaEnTransicion() const {
    return enTransicion;
}

float Oleada::getTemporizadorTransicion() const {
    return timerTransicion;
}
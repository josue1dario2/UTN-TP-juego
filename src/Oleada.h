#pragma once
// Oleada.h - Sistema de oleadas de zombies

#include "Zombie.h"

class Oleada {
private:
    int oleadaActual;
    int totalZombies;
    int zombiesSpawnados;
    int zombiesMuertos;

    float intervaloSpawn;
    float timerSpawn;

    bool oleadaActiva;

    float tiempoEntreOleadas;
    float timerTransicion;
    bool enTransicion;

public:
    Oleada();
    void iniciarOleada(int numeroOleada);
    void actualizar(float dt);
    bool debeSpawnearZombie();
    void zombieMuerto();
    bool oleadaCompletada() const;
    TipoZombie getZombieTypeForWave() const;

    int getOleadaActual() const;
    int getZombiesRestantes() const;
    bool estaOleadaActiva() const;
    bool estaEnTransicion() const;
    float getTemporizadorTransicion() const;
};
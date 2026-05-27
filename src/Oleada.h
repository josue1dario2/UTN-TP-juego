#pragma once
// Oleada.h - Clase oleada segun diagrama UML

#include <vector>

// Forward declaration
enum class TipoZombie;

class Zombie;

class Oleada {
private:
    int numero;
    int enemigosTotales;
    int enemigosVivos;
    bool completada;
    std::vector<Zombie*> zombies;

public:
    Oleada();
    ~Oleada();

    // Segun diagrama UML
    void iniciar(int numeroOleada);
    void actualizar();

    // Metodos que usa Juego.cpp (compatibilidad)
    void iniciarOleada(int numeroOleada);
    void actualizar(float dt);
    bool oleadaCompletada() const;
    bool debeSpawnearZombie();
    void zombieMuerto();
    int getOleadaActual() const;
    int getZombiesRestantes() const;
    bool estaOleadaActiva() const;
    TipoZombie getZombieTypeForWave() const;

    int getNumero() const;
    int getEnemigosVivos() const;
    bool estaCompletada() const;
};

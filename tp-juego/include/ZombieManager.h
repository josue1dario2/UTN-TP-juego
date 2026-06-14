#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Zombie.h"
#include "ObjetoMapa.h"
#include "Proyectil.h"

class Personaje;

class ZombieManager {
private:
    std::vector<Zombie> zombies;
    std::vector<sf::FloatRect> zonasSpawn;
    std::vector<int> indicesZonasActivas;

    // Control de tiempo y oleadas
    int oleadaActual;
    bool enPeriodoDescanso;
    float cronometroDescanso;
    float temporizadorSpawn;
    int zombiesRestantesPorCrear;
    float cronometroOleada;

    const float TIEMPO_DESCANSO = 10.f;    // Los 10 segundos de la consigna
    const float TIEMPO_MAX_OLEADA = 90.f;  // Tiempo límite para forzar la ronda
    const float FRECUENCIA_SPAWN = 0.5f;   

    void seleccionarZonaSpawnOptima(const sf::Vector2f& posJugador);
    void intentarSpawnearUnZombie(const std::vector<ObjetoMapa>& obstaculos);

public:
    ZombieManager();

    void inicializarZonasSpawn(const std::vector<sf::FloatRect>& zonas);

    void actualizar(float deltaTime, Personaje& jugador, const std::vector<ObjetoMapa>& obstaculos, std::vector<Proyectil>& proyectiles);

    void dibujarZombies(sf::RenderWindow& ventana);

    std::vector<sf::FloatRect> getHitboxesZombies() const;
    const std::vector<Zombie>& getZombies() const { return zombies; }
};

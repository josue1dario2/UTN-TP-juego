#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Zombie.h"
#include "ObjetoMapa.h"
#include "Proyectil.h"

// Forward declaration of Personaje to avoid circular dependency
class Personaje;

class ZombieManager {
private:
    std::vector<Zombie> zombies;
    std::vector<sf::FloatRect> zonasSpawn;
    int indiceZonaActiva;

public:
    ZombieManager();

    // Spawning & Initializing
    void inicializarZonasSpawn(const std::vector<sf::FloatRect>& zonas);
    void inicializarZombies(int cantidadDeseada, const Personaje& jugador, const std::vector<ObjetoMapa>& obstaculos);

    // Updates & Collision Management
    void actualizar(float deltaTime, Personaje& jugador, const std::vector<ObjetoMapa>& obstaculos, std::vector<Proyectil>& proyectiles);

    // Drawing
    void dibujarZombies(sf::RenderWindow& ventana);
    void dibujarZonasSpawn(sf::RenderWindow& ventana);

    // Getters for integration with Juego/Player colisions
    std::vector<sf::FloatRect> getHitboxesZombies() const;
    const std::vector<Zombie>& getZombies() const { return zombies; }
};

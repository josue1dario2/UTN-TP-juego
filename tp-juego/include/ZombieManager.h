#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Zombie.h"
#include "ObjetoMapa.h"
#include "Proyectil.h"
#include "Constantes.h"

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



    void seleccionarZonaSpawnOptima(const sf::Vector2f& posJugador);
    void intentarSpawnearUnZombie(const std::vector<ObjetoMapa>& obstaculos);

public:
    ZombieManager();

    void inicializarZonasSpawn(const std::vector<sf::FloatRect>& zonas);

    void actualizar(float deltaTime, Personaje& jugador, const std::vector<ObjetoMapa>& obstaculos, std::vector<Proyectil>& proyectiles, std::vector<Mina>& trampas);

    void dibujarZombies(sf::RenderWindow& ventana);

    std::vector<sf::FloatRect> getHitboxesZombies() const;

    int getOleadaActual() const { return oleadaActual; }
    void cargarOleada(int oleada);
    bool getEnPeriodoDescanso() const { return enPeriodoDescanso; }
    float getCronometroDescanso() const { return cronometroDescanso; }
    float getTiempoTotalDescanso() const { return Config::Gameplay::TiempoDescanso; }
    int getZombiesActivos() const { return zombies.size(); }
    int getZombiesRestantesPorCrear() const { return zombiesRestantesPorCrear; }
    std::vector<Zombie>& getZombies() { return zombies; }
    
    int getZombiesEliminados() const { return zombiesEliminados; }
    void resetZombiesEliminados() { zombiesEliminados = 0; }

private:
    int zombiesEliminados = 0;
};

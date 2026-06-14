#include "../include/ZombieManager.h"
#include "../include/Personaje.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

ZombieManager::ZombieManager() {
    oleadaActual = 1;
    enPeriodoDescanso = false;
    cronometroDescanso = 0.f;
    temporizadorSpawn = 0.f;
    zombiesRestantesPorCrear = 5;
    cronometroOleada = 0.f;
    std::cout << "OLEADA Inicio de la Oleada " << oleadaActual << " (Zombies a crear: " << zombiesRestantesPorCrear << ")" << std::endl;
}

// Guarda en el gestor las áreas rectangulares del mapa donde pueden aparecer (spawnear) los zombies
void ZombieManager::inicializarZonasSpawn(const std::vector<sf::FloatRect>& zonas) {
    zonasSpawn = zonas;
}

// Selecciona las 3 zonas de spawn más lejanas al jugador
void ZombieManager::seleccionarZonaSpawnOptima(const sf::Vector2f& posJugador) {
    if (zonasSpawn.empty()) return;

    struct ZonaDistancia {
        int indice;
        float distSq;
    };

    std::vector<ZonaDistancia> listado;
    listado.reserve(zonasSpawn.size());

    for (size_t i = 0; i < zonasSpawn.size(); ++i) {
        const auto &zona = zonasSpawn[i];
        sf::Vector2f centroZona(zona.left + zona.width / 2.f, zona.top + zona.height / 2.f);
        sf::Vector2f diff = centroZona - posJugador;
        float distSq = diff.x * diff.x + diff.y * diff.y;
        listado.push_back({static_cast<int>(i), distSq});
    }

    // Ordenar de mayor a menor distancia (de mayor a menor distSq)
    std::sort(listado.begin(), listado.end(), [](const ZonaDistancia& a, const ZonaDistancia& b) {
        return a.distSq > b.distSq;
    });

    indicesZonasActivas.clear();
    size_t limite = std::min(static_cast<size_t>(3), listado.size());
    for (size_t i = 0; i < limite; ++i) {
        indicesZonasActivas.push_back(listado[i].indice);
    }
}

// Intenta colocar un zombie en un punto seguro de una de las 3 zonas activas seleccionadas al azar
void ZombieManager::intentarSpawnearUnZombie(const std::vector<ObjetoMapa>& obstaculos) {
    if (indicesZonasActivas.empty() || zonasSpawn.empty()) return;

    float distMinZombies = 100.f; // Distancia mínima permitida entre zombies
    int maxIntentos = 100;        // Intentos máximos
    
    int indexZonaElegida = indicesZonasActivas[std::rand() % indicesZonasActivas.size()];
    const sf::FloatRect &zonaElegida = zonasSpawn[indexZonaElegida];

    bool spawnValido = false;
    sf::Vector2f spawn(0.f, 0.f);

    for (int intento = 0; intento < maxIntentos; ++intento) {
        // Generar coordenadas aleatorias dentro de la zona elegida
        float rx = zonaElegida.left + static_cast<float>(std::rand() % static_cast<int>(zonaElegida.width));
        float ry = zonaElegida.top + static_cast<float>(std::rand() % static_cast<int>(zonaElegida.height));
        spawn = sf::Vector2f(rx, ry);

        // A. Verificar colisión del nuevo zombie con los obstáculos
        sf::FloatRect hitboxZombie(spawn.x - 13.f, spawn.y - 16.8f, 26.f, 33.6f);
        bool colisionaConObstaculo = false;
        for (const auto &obstaculo : obstaculos) {
            if (hitboxZombie.intersects(obstaculo.getHitbox())) {
                colisionaConObstaculo = true;
                break;
            }
        }
        if (colisionaConObstaculo) {
            continue;
        }

        // B. Verificar que no esté demasiado cerca de otro zombie
        bool colisionaConZombie = false;
        for (const auto &zombieExistente : zombies) {
            sf::Vector2f diffZombie = spawn - zombieExistente.getPosicion();
            float distZombieSq = diffZombie.x * diffZombie.x + diffZombie.y * diffZombie.y;
            if (distZombieSq < distMinZombies * distMinZombies) {
                colisionaConZombie = true;
                break;
            }
        }
        if (colisionaConZombie) {
            continue;
        }

        spawnValido = true;
        break;
    }

    if (spawnValido) {
        zombies.emplace_back(1, 100, 10, 80.f);
        Zombie &refZombie = zombies.back();
        refZombie.cargarTextura("assets/zombie.png");
        refZombie.centrarOrigen();
        refZombie.escalarSprite(1.2f, 1.2f);
        refZombie.setHitbox(26.f, 33.6f);
        refZombie.setPosicionCentrado(spawn.x, spawn.y);

        zombiesRestantesPorCrear--;
    }
}

// Retorna las hitboxes de todos los zombies activos que están vivos (útil para calcular las colisiones del jugador)
std::vector<sf::FloatRect> ZombieManager::getHitboxesZombies() const {
    std::vector<sf::FloatRect> hitboxes;
    for (const auto &zombie : zombies) {
        if (!zombie.muerto()) {
            hitboxes.push_back(zombie.getHitbox());
        }
    }
    return hitboxes;
}

// Bucle de actualización principal: controla movimiento, lógica de ataque, colisiones de balas y limpieza de cadáveres
void ZombieManager::actualizar(float deltaTime, Personaje& jugador, const std::vector<ObjetoMapa>& obstaculos, std::vector<Proyectil>& proyectiles) {
    
    // Si es el primer frame y no se han seleccionado zonas de spawn, seleccionarlas
    if (indicesZonasActivas.empty() && !zonasSpawn.empty()) {
        seleccionarZonaSpawnOptima(jugador.getPosicion());
    }

    // Máquina de estados del gestor de oleadas
    if (enPeriodoDescanso) {
        cronometroDescanso += deltaTime;
        if (cronometroDescanso >= TIEMPO_DESCANSO) {
            enPeriodoDescanso = false;
            oleadaActual++;
            int nuevosZombies = 5 + (oleadaActual - 1) * 3;
            zombiesRestantesPorCrear = nuevosZombies;
            zombies.reserve(nuevosZombies + 10);
            
            // Recalcular las 3 zonas más alejadas basándose en la posición del jugador
            seleccionarZonaSpawnOptima(jugador.getPosicion());

            std::cout << "TREGUA Fin de la tregua. Iniciando Oleada " << oleadaActual << "..." << std::endl;
            std::cout << "OLEADA Inicio de la Oleada " << oleadaActual << " (Zombies a crear: " << zombiesRestantesPorCrear << ")" << std::endl;
            
            cronometroOleada = 0.f;
            temporizadorSpawn = 0.f;
        }
    } else {
        cronometroOleada += deltaTime;
        
        // Generar zombies de manera paulatina
        if (zombiesRestantesPorCrear > 0) {
            temporizadorSpawn += deltaTime;
            if (temporizadorSpawn >= FRECUENCIA_SPAWN) {
                intentarSpawnearUnZombie(obstaculos);
                temporizadorSpawn = 0.f;
            }
        }

        // Comprobación de fin de oleada (todos muertos o tiempo límite alcanzado)
        if ((zombies.empty() && zombiesRestantesPorCrear == 0) || cronometroOleada >= TIEMPO_MAX_OLEADA) {
            enPeriodoDescanso = true;
            cronometroDescanso = 0.f;
            cronometroOleada = 0.f;
            std::cout << "TREGUA Oleada finalizada/tiempo limite alcanzado. Comenzando tregua de " << TIEMPO_DESCANSO << " segundos." << std::endl;
        }
    }

    // 1. Actualizar movimiento y logica de evasión/persecución para cada zombie
    for (auto &zombie : zombies) {
        zombie.actualizar(deltaTime, jugador.estaVivo() ? jugador.getHitbox() : sf::FloatRect(), obstaculos, zombies);
    }

    // 2. Procesar colisiones de Proyectiles (Balas) contra los Zombies
    for (auto &proyectil : proyectiles) {
        for (auto &zombie : zombies) {
            if (!zombie.muerto() && proyectil.getHitbox().intersects(zombie.getHitbox())) {
                zombie.quitarVida(proyectil.getDanio());
                proyectil.desactivar(); // Marcar el proyectil para ser destruido
                std::cout << "IMPACTO Zombie recibio " << proyectil.getDanio() 
                          << " de danio. Vida restante: " << zombie.getVida() << std::endl;
                
                if (zombie.muerto()) {
                    std::cout << "MUERTE Un Zombie ha sido eliminado!" << std::endl;
                }
                break; // El proyectil chocó, no puede golpear a más zombies en este frame
            }
        }
    }

    // 3. Procesar colisiones de Zombies contra el Jugador (Ataque y daño al jugador)
    for (auto &zombie : zombies) {
        if (!zombie.muerto() && jugador.estaVivo()) {
            // Expandimos la hitbox del jugador ligeramente para dar tolerancia al área de contacto
            sf::FloatRect expandedHitbox = jugador.getHitbox();
            expandedHitbox.left -= 2.f;
            expandedHitbox.top -= 2.f;
            expandedHitbox.width += 4.f;
            expandedHitbox.height += 4.f;

            if (expandedHitbox.intersects(zombie.getHitbox())) {
                if (zombie.puedeAtacar()) {
                    jugador.recibirDanio(zombie.getAtaque());
                    zombie.reiniciarTiempoAtaque(); // Reinicia el cronómetro del cooldown de ataque del zombie
                    std::cout << "ATAQUE Zombie ataco al jugador! Vida del jugador: " << jugador.getVidaActual() 
                              << " | Armadura: " << jugador.getArmaduraActual() << std::endl;

                    if (!jugador.estaVivo()) {
                        std::cout << "GAME OVER El jugador ha muerto. Partida terminada." << std::endl;
                    }
                }
            }
        }
    }

    // 4. Limpieza: Elimina del vector a todos los zombies marcados como muertos para liberar memoria
    zombies.erase(std::remove_if(zombies.begin(), zombies.end(), [](const Zombie &z) { return z.muerto(); }), zombies.end());
}

// Dibuja en pantalla todos los zombies gestionados
void ZombieManager::dibujarZombies(sf::RenderWindow& ventana) {
    for (auto &zombie : zombies) {
        zombie.dibujar(ventana);
    }
}

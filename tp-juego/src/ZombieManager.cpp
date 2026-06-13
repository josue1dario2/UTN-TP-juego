#include "../include/ZombieManager.h"
#include "../include/Personaje.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

ZombieManager::ZombieManager() {
    indiceZonaActiva = -1;
}

// Guarda en el gestor las áreas rectangulares del mapa donde pueden aparecer (spawnear) los zombies
void ZombieManager::inicializarZonasSpawn(const std::vector<sf::FloatRect>& zonas) {
    zonasSpawn = zonas;
}

// Selecciona la zona de spawn más lejana al jugador y genera en ella los zombies en posiciones aleatorias seguras
void ZombieManager::inicializarZombies(int cantidadDeseada, const Personaje& jugador, const std::vector<ObjetoMapa>& obstaculos) {
    int zombiesCreados = 0;
    float distMinZombies = 100.f; // Distancia mínima permitida entre zombies al nacer para evitar superposiciones
    int maxIntentos = 100;        // Intentos máximos para encontrar un punto de spawn válido antes de rendirse

    // Reserva memoria en el vector para optimizar el rendimiento al añadir elementos
    zombies.reserve(cantidadDeseada + 10);

    // 1. Encontrar la zona de spawn que esté a mayor distancia del jugador
    float maxDistSq = -1.f;
    int indexMasLejano = 0;
    sf::Vector2f posJugador = jugador.getPosicion();

    for (size_t i = 0; i < zonasSpawn.size(); ++i) {
        const auto &zona = zonasSpawn[i];
        sf::Vector2f centroZona(zona.left + zona.width / 2.f, zona.top + zona.height / 2.f);
        sf::Vector2f diff = centroZona - posJugador;
        float distSq = diff.x * diff.x + diff.y * diff.y; // Distancia al cuadrado (más rápido de calcular que usar raíz cuadrada)
        if (distSq > maxDistSq) {
            maxDistSq = distSq;
            indexMasLejano = i;
        }
    }
    indiceZonaActiva = indexMasLejano;
    const sf::FloatRect &zonaActiva = zonasSpawn[indiceZonaActiva];

    // 2. Intentar colocar cada zombie en un punto seguro de la zona seleccionada
    while (zombiesCreados < cantidadDeseada) {
        bool spawnValido = false;
        sf::Vector2f spawn(0.f, 0.f);

        for (int intento = 0; intento < maxIntentos; ++intento) {
            // Generar coordenadas aleatorias dentro de los límites de la zona activa
            float rx = zonaActiva.left + static_cast<float>(std::rand() % static_cast<int>(zonaActiva.width));
            float ry = zonaActiva.top + static_cast<float>(std::rand() % static_cast<int>(zonaActiva.height));
            spawn = sf::Vector2f(rx, ry);

            // A. Verificar colisión del nuevo zombie con los obstáculos del mapa
            sf::FloatRect hitboxZombie(spawn.x - 13.f, spawn.y - 16.8f, 26.f, 33.6f);
            bool colisionaConObstaculo = false;
            for (const auto &obstaculo : obstaculos) {
                if (hitboxZombie.intersects(obstaculo.getHitbox())) {
                    colisionaConObstaculo = true;
                    break;
                }
            }
            if (colisionaConObstaculo) {
                continue; // Si colisiona con una pared/obstáculo, reintenta en otra posición
            }

            // B. Verificar que el zombie no aparezca demasiado cerca de otro zombie ya creado
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
                continue; // Si está muy encimado a otro, reintenta
            }

            spawnValido = true;
            break; // Posición válida encontrada, salir del bucle de intentos
        }

        // Si la posición fue válida, instanciar y configurar el objeto Zombie
        if (spawnValido) {
            zombies.emplace_back(1, 100, 10, 80.f);
            Zombie &refZombie = zombies.back();
            refZombie.cargarTextura("assets/zombie.png");
            refZombie.centrarOrigen();
            refZombie.escalarSprite(1.2f, 1.2f);
            refZombie.setHitbox(26.f, 33.6f);
            refZombie.setPosicionCentrado(spawn.x, spawn.y);

            zombiesCreados++;
        } else {
            break; // Evita bucle infinito si el espacio está muy congestionado
        }
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

// Dibuja en pantalla las cajas delimitadoras de las zonas de spawn (verde para la activa/más lejana, azul para las inactivas)
void ZombieManager::dibujarZonasSpawn(sf::RenderWindow& ventana) {
    for (size_t i = 0; i < zonasSpawn.size(); ++i) {
        const auto &zona = zonasSpawn[i];
        sf::RectangleShape rectSpawn(sf::Vector2f(zona.width, zona.height));
        rectSpawn.setPosition(zona.left, zona.top);
        if (static_cast<int>(i) == indiceZonaActiva) {
            rectSpawn.setFillColor(sf::Color(0, 255, 0, 30)); // Relleno verde transparente
            rectSpawn.setOutlineColor(sf::Color::Green);
        } else {
            rectSpawn.setFillColor(sf::Color(0, 0, 255, 15)); // Relleno azul transparente
            rectSpawn.setOutlineColor(sf::Color(0, 0, 255, 100));
        }
        rectSpawn.setOutlineThickness(3.f);
        ventana.draw(rectSpawn);
    }
}

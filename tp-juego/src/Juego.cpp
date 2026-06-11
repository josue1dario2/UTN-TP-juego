#include "../include/Juego.h"
#include <cstdlib>
#include <ctime>

Juego::Juego(int idJug, int idArma, std::string nombre, float vida, float armadura, float velocidad, float cooldown) :
    jugador(idJug, idArma, nombre, vida, armadura, velocidad, cooldown) {

  deltaTime = 0.f;

  sf::VideoMode modoEscritorio = sf::VideoMode::getDesktopMode();

  ventana.create(modoEscritorio, "Mi Juego", sf::Style::Fullscreen);

  ventana.setMouseCursorVisible(false); // Ocultar el cursor estándar de la computadora

  ventana.setFramerateLimit(60);

  vista.setSize(1280.f, 720.f);
  vista.setCenter(640.f, 360.f);
  ventana.setView(vista);

  // Inicialización de elementos del terreno
  inicializarObstaculos(obstaculos);

  // Definimos 6 zonas de spawn en los corredores laterales libres de obstáculos
  zonasSpawn = {
      sf::FloatRect(200.f, 100.f, 400.f, 350.f),  // Superior Izquierda
      sf::FloatRect(3250.f, 100.f, 400.f, 350.f), // Superior Derecha
      sf::FloatRect(200.f, 850.f, 400.f, 350.f),  // Central Izquierda
      sf::FloatRect(3250.f, 850.f, 400.f, 350.f), // Central Derecha
      sf::FloatRect(200.f, 1600.f, 400.f, 350.f), // Inferior Izquierda
      sf::FloatRect(3250.f, 1600.f, 400.f, 350.f) // Inferior Derecha
  };
  indiceZonaActiva = -1;

  texturaProyectil.loadFromFile("assets/bala.png");
}

void Juego::inicializarObstaculos(std::vector<ObjetoMapa> &obstaculos) {
  obstaculos.reserve(40);
  // casucha
  obstaculos.emplace_back();
  obstaculos.back().cargarTextura("assets/casa.png");
  obstaculos.back().centrarOrigen();
  obstaculos.back().escalarSprite(3.f, 3.f);
  obstaculos.back().setHitbox(64.f * 3.f, 54.f * 3.f);
  obstaculos.back().setPosicionCentrado(1920.f, 1080.f);

  obstaculos.emplace_back();
  obstaculos.back().setHitbox(1058.f, 258.f);
  obstaculos.back().setPosicion(640.f, 540.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(1058.f, 258.f);
  obstaculos.back().setPosicion(640.f, 1300.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(1058.f, 258.f);
  obstaculos.back().setPosicion(2170.f, 1300.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(1058.f, 258.f);
  obstaculos.back().setPosicion(2170.f, 540.f);

  obstaculos.emplace_back();
  obstaculos.back().setHitbox(1058.f, 146.f);
  obstaculos.back().setPosicion(2170.f, 2014.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(1058.f, 146.f);
  obstaculos.back().setPosicion(636.f, 2014.f);

  obstaculos.emplace_back();
  obstaculos.back().setHitbox(1058.f, 72.f);
  obstaculos.back().setPosicion(2170.f, 0.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(1058.f, 72.f);
  obstaculos.back().setPosicion(640.f, 0.f);

  obstaculos.emplace_back();
  obstaculos.back().setHitbox(168.f, 72.f);
  obstaculos.back().setPosicion(0.f, 0.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(168.f, 258.f);
  obstaculos.back().setPosicion(0.f, 540.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(168.f, 258.f);
  obstaculos.back().setPosicion(0.f, 1300.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(168.f, 146.f);
  obstaculos.back().setPosicion(0.f, 2014.f);

  obstaculos.emplace_back();
  obstaculos.back().setHitbox(168.f, 72.f);
  obstaculos.back().setPosicion(3672.f, 0.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(168.f, 258.f);
  obstaculos.back().setPosicion(3672.f, 540.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(168.f, 258.f);
  obstaculos.back().setPosicion(3672.f, 1300.f);
  obstaculos.emplace_back();
  obstaculos.back().setHitbox(168.f, 146.f);
  obstaculos.back().setPosicion(3672.f, 2014.f);
}

// Ejecuta el bucle principal del juego
void Juego::iniciar() {
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  texturaMapa.loadFromFile("assets/mapa.png");
  spriteMapa.setTexture(texturaMapa);

  inicializarZombies();

  while (ventana.isOpen()) {
    // obtiene cuánto tiempo pasó desde el frame anterior y reinicia el reloj
    deltaTime = relojDelta.restart().asSeconds();

    procesarEventos();
    actualizar();
    renderizar();
  }
}

// Maneja eventos de ventana e input del usuario
void Juego::procesarEventos() {
  sf::Event evento;
  while (ventana.pollEvent(evento)) {
    if (evento.type == sf::Event::Closed) {
      ventana.close();
    }
    if (evento.type == sf::Event::KeyPressed &&
        evento.key.code == sf::Keyboard::Escape) {
      ventana.close();
    }
  }
}

// Actualiza la logica del juego
void Juego::actualizar() {
    jugador.actualizar(deltaTime, obstaculos);
    jugador.getArma().actualizar(deltaTime, mira.getPosicion(), jugador.getPosicion(), proyectiles,texturaProyectil);

    vista.setSize(1280.f * jugador.getMultiplicadorZoom(), 720.f * jugador.getMultiplicadorZoom());
    
    for(auto& proyectil : proyectiles) {
        proyectil.actualizar(deltaTime, obstaculos);
  // Obtener hitboxes de zombies vivos para colision del jugador
  std::vector<sf::FloatRect> hitboxesZombies;
  for (const auto &zombie : zombies) {
    if (!zombie.muerto()) {
      hitboxesZombies.push_back(zombie.getHitbox());
    }
  }

  // Logica de movimiento del jugador (solo si esta vivo)
  if (jugador.estaVivo()) {
    jugador.actualizar(deltaTime, obstaculos, hitboxesZombies);
    jugador.getArma().actualizar(deltaTime, mira.getPosicion(), jugador.getPosicion(), proyectiles, texturaProyectil);
  }

  for (auto &proyectil : proyectiles) {
    proyectil.actualizar(deltaTime, obstaculos);
  }

  // 1. Deteccion de colision de Proyectil vs Zombie
  for (auto &proyectil : proyectiles) {
    for (auto &zombie : zombies) {
      if (!zombie.muerto() && proyectil.getHitbox().intersects(zombie.getHitbox())) {
        zombie.quitarVida(proyectil.getDanio());
        proyectil.desactivar();
        std::cout << "IMPACTO Zombie recibio " << proyectil.getDanio() 
                  << " de danio. Vida restante: " << zombie.getVida() << std::endl;
        
        if (zombie.muerto()) {
          std::cout << "MUERTE Un Zombie ha sido eliminado!" << std::endl;
        }
        break;
      }
    }
  }

  proyectiles.erase(std::remove_if(proyectiles.begin(), proyectiles.end(), [](const Proyectil &p) { return p.debeDestruirse(); }), proyectiles.end());

  // Chequear ataque de los zombies usando un hitbox ligeramente expandido para tolerancia
  for (auto &zombie : zombies) {
    if (!zombie.muerto() && jugador.estaVivo()) {
      sf::FloatRect expandedHitbox = jugador.getHitbox();
      expandedHitbox.left -= 2.f;
      expandedHitbox.top -= 2.f;
      expandedHitbox.width += 4.f;
      expandedHitbox.height += 4.f;

      if (expandedHitbox.intersects(zombie.getHitbox())) {
        // Logica de ataque del zombie al jugador con Cooldown
        if (zombie.puedeAtacar()) {
          jugador.recibirDanio(zombie.getAtaque());
          zombie.reiniciarTiempoAtaque();
          std::cout << "ATAQUE Zombie ataco al jugador! Vida del jugador: " << jugador.getVidaActual() 
                    << " | Armadura: " << jugador.getArmaduraActual() << std::endl;

          if (!jugador.estaVivo()) {
            std::cout << "GAME OVER El jugador ha muerto. Partida terminada." << std::endl;
          }
        }
      }
    }
  }

  // Limpiar zombies muertos del vector
  zombies.erase(std::remove_if(zombies.begin(), zombies.end(), [](const Zombie &z) { return z.muerto(); }), zombies.end());

  auxVistaX = jugador.getPosicion().x;
  auxVistaY = jugador.getPosicion().y;

  // Limitar el centro de la camara para que nunca muestre el exterior (el vacio negro)
  if (auxVistaX < vista.getSize().x / 2.f)
    auxVistaX = vista.getSize().x / 2.f;
  if (auxVistaX > texturaMapa.getSize().x - vista.getSize().x / 2.f)
    auxVistaX = texturaMapa.getSize().x - vista.getSize().x / 2.f;

  if (auxVistaY < vista.getSize().y / 2.f)
    auxVistaY = vista.getSize().y / 2.f;
  if (auxVistaY > texturaMapa.getSize().y - vista.getSize().y / 2.f)
    auxVistaY = texturaMapa.getSize().y - vista.getSize().y / 2.f;

  vista.setCenter(auxVistaX, auxVistaY);
  ventana.setView(vista);

  // Actualizar la mira personalizada y hacerla girar
  mira.actualizar(ventana, deltaTime);

  // Actualizar zombies
  for (auto &zombie : zombies) {
    zombie.actualizar(deltaTime, jugador.estaVivo() ? jugador.getHitbox() : sf::FloatRect(), obstaculos, zombies);
  }
}

// Dibuja todos los elementos en pantalla
void Juego::renderizar() {
  ventana.clear();

  // acá se dibujan las cosas
  ventana.draw(spriteMapa);

  // La zona activa (la más lejana) se dibuja en verde. Las zonas inactivas se dibujan en azul.
  for (size_t i = 0; i < zonasSpawn.size(); ++i) {
    const auto &zona = zonasSpawn[i];
    sf::RectangleShape rectSpawn(sf::Vector2f(zona.width, zona.height));
    rectSpawn.setPosition(zona.left, zona.top);
    if (static_cast<int>(i) == indiceZonaActiva) {
      rectSpawn.setFillColor(
          sf::Color(0, 255, 0, 30)); // Relleno verde muy transparente
      rectSpawn.setOutlineColor(sf::Color::Green);
    } else {
      rectSpawn.setFillColor(
          sf::Color(0, 0, 255, 15)); // Relleno azul sumamente transparente
      rectSpawn.setOutlineColor(sf::Color(0, 0, 255, 100)); // Borde azul
    }
    rectSpawn.setOutlineThickness(3.f);
    ventana.draw(rectSpawn);
  }

  // Dibuja los obstáculos con un bucle
  for (auto &obstaculo : obstaculos) {
    obstaculo.dibujar(ventana);
  }

  for (auto &proyectil : proyectiles) {
    proyectil.dibujar(ventana);
  }

  if (jugador.estaVivo()) {
    jugador.dibujar(ventana);
    jugador.getArma().dibujar(ventana);
  }

  // Dibujar todos los zombies
  for (auto &zombie : zombies) {
    zombie.dibujar(ventana);
  }

  // Dibujar el puntero personalizado (la mira giratoria) encima de todo
  mira.dibujar(ventana);

  ventana.display();
}

void Juego::inicializarZombies() {
  int cantidadZombiesDeseada = 5;
  int zombiesCreados = 0;
  float distMinZombies = 100.f;
  int maxIntentos = 100;

  // Reservamos memoria para evitar reasignación y pérdida de punteros de textura
  zombies.reserve(cantidadZombiesDeseada + 10);

  // Buscamos la zona de spawn que esté más lejana del jugador
  float maxDistSq = -1.f;
  int indexMasLejano = 0;
  sf::Vector2f posJugador = jugador.getPosicion();

  for (size_t i = 0; i < zonasSpawn.size(); ++i) {
    const auto &zona = zonasSpawn[i];
    sf::Vector2f centroZona(zona.left + zona.width / 2.f,
                            zona.top + zona.height / 2.f);
    sf::Vector2f diff = centroZona - posJugador;
    float distSq = diff.x * diff.x + diff.y * diff.y;
    if (distSq > maxDistSq) {
      maxDistSq = distSq;
      indexMasLejano = i;
    }
  }
  indiceZonaActiva = indexMasLejano;
  const sf::FloatRect &zonaActiva = zonasSpawn[indiceZonaActiva];

  while (zombiesCreados < cantidadZombiesDeseada) {
    bool spawnValido = false;
    sf::Vector2f spawn(0.f, 0.f);

    for (int intento = 0; intento < maxIntentos; ++intento) {
      float rx =
          zonaActiva.left +
          static_cast<float>(std::rand() % static_cast<int>(zonaActiva.width));
      float ry =
          zonaActiva.top +
          static_cast<float>(std::rand() % static_cast<int>(zonaActiva.height));
      spawn = sf::Vector2f(rx, ry);

      // 1. Verificar colisión con obstáculos
      sf::FloatRect hitboxZombie(spawn.x - 13.f, spawn.y - 16.8f, 26.f, 33.6f);
      bool colisionaConObstaculo = false;
      for (const auto &obstaculo : obstaculos) {
        if (hitboxZombie.intersects(obstaculo.getHitbox())) {
          colisionaConObstaculo = true;
          break;
        }
      }
      if (colisionaConObstaculo) {
        continue; // Colisiona con obstáculo
      }

      // 2. Verificar si está muy cerca de otro zombie ya creado (superposición)
      bool colisionaConZombie = false;
      for (const auto &zombieExistente : zombies) {
        sf::Vector2f diffZombie = spawn - zombieExistente.getPosicion();
        float distZombieSq =
            diffZombie.x * diffZombie.x + diffZombie.y * diffZombie.y;
        if (distZombieSq < distMinZombies * distMinZombies) {
          colisionaConZombie = true;
          break;
        }
      }
      if (colisionaConZombie) {
        continue; // Demasiado cerca de otro zombie
      }

      // Si pasa todas las validaciones, el punto de spawn es válido
      spawnValido = true;
      break;
    }

    // Si encontramos una posición válida, creamos el zombie
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
      // Si superamos el número de intentos, salimos para evitar bloqueo
      break;
    }
  }
}
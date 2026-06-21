#include "../include/Juego.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

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
  std::vector<sf::FloatRect> zonasSpawn = {
      sf::FloatRect(200.f, 100.f, 400.f, 350.f),  // Superior Izquierda
      sf::FloatRect(3250.f, 100.f, 400.f, 350.f), // Superior Derecha
      sf::FloatRect(200.f, 850.f, 400.f, 350.f),  // Central Izquierda
      sf::FloatRect(3250.f, 850.f, 400.f, 350.f), // Central Derecha
      sf::FloatRect(200.f, 1600.f, 400.f, 350.f), // Inferior Izquierda
      sf::FloatRect(3250.f, 1600.f, 400.f, 350.f) // Inferior Derecha
  };
  zombieManager.inicializarZonasSpawn(zonasSpawn);

  texturaProyectil.loadFromFile("assets/armas/bala.png");

  hud.inicializar();

  trazaMosin.setFillColor(sf::Color::White);
  trazaMosin.setSize(sf::Vector2f(3000.f, 3.f));
  trazaMosin.setOrigin(0.f, 1.5f);
}

void Juego::inicializarObstaculos(std::vector<ObjetoMapa> &obstaculos) {
  obstaculos.reserve(40);
  // casucha
  /*
  obstaculos.emplace_back();
  obstaculos.back().cargarTextura("assets/varios/casa.png");
  obstaculos.back().centrarOrigen();
  obstaculos.back().escalarSprite(3.f, 3.f);
  obstaculos.back().setHitbox(64.f * 3.f, 54.f * 3.f);
  obstaculos.back().setPosicionCentrado(1920.f, 1080.f);
  */

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

  texturaMapa.loadFromFile("assets/varios/mapa.png");
  spriteMapa.setTexture(texturaMapa);
  proyectiles.reserve(100);

  trampas.emplace_back(sf::Vector2f(1920,1080));
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
  // Obtener hitboxes de zombies vivos para colision del jugador
  std::vector<sf::FloatRect> hitboxesZombies = zombieManager.getHitboxesZombies();

  // -------- Logica de movimiento del jugador (solo si esta vivo) --------
  if (jugador.estaVivo()) {
    jugador.actualizar(deltaTime, obstaculos, hitboxesZombies, mira.getPosicion(), trampas);
    jugador.getArma().actualizar(deltaTime, mira.getPosicion(), jugador.getPosicion(), proyectiles, texturaProyectil);
    vista.setSize(1280.f * jugador.getMultiplicadorZoom(), 720.f * jugador.getMultiplicadorZoom());
  }

  // -------- Proyectiles y armas --------
  procesarRayCast();

  for (auto &proyectil : proyectiles) {
    proyectil.actualizar(deltaTime, obstaculos);
  }

  proyectiles.erase(std::remove_if(proyectiles.begin(), proyectiles.end(), [](const Proyectil &p) { return p.debeDestruirse(); }), proyectiles.end());

  // -------- trampas --------
  for (auto &trampa : trampas) {
    trampa.actualizar(deltaTime, zombieManager.getHitboxesZombies());
  }

  trampas.erase(std::remove_if(trampas.begin(), trampas.end(), [](const Mina &m) { return m.debeDestruirse(); }), trampas.end());

  // -------- Lógica de zombies y colisión de balas delegada en ZombieManager --------
  zombieManager.actualizar(deltaTime, jugador, obstaculos, proyectiles, trampas);

  auxVistaX = jugador.getPosicion().x;
  auxVistaY = jugador.getPosicion().y;

  // -------- CAMARA --------
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

  hud.actualizar(jugador, zombieManager);
}

// Dibuja todos los elementos en pantalla
void Juego::renderizar() {
  ventana.clear();

  // acá se dibujan las cosas
  ventana.draw(spriteMapa);

  for (auto &trampa : trampas) {
    trampa.dibujar(ventana);
  }

  // Dibuja los obstáculos con un bucle
  for (auto &obstaculo : obstaculos) {
    obstaculo.dibujar(ventana);
  }

  for (auto &proyectil : proyectiles) {
    proyectil.dibujar(ventana);
  }

  if(mostrarTrazaMosin) {
    ventana.draw(trazaMosin);
  }
  
  if (jugador.estaVivo()) {
    jugador.dibujar(ventana);
    jugador.getArma().dibujar(ventana);
  }

  // Dibujar todos los zombies
  zombieManager.dibujarZombies(ventana);

  // Dibujar el puntero personalizado (la mira giratoria) encima de todo
  mira.dibujar(ventana);

  hud.dibujar(ventana);

  ventana.display();
}

void Juego::procesarRayCast(){

  if(mostrarTrazaMosin) {
    tiempoTrazaMosin -= deltaTime;
    
    if(tiempoTrazaMosin <= 0.f) {
      mostrarTrazaMosin = false;
    }
  }
  
  if(jugador.getArma().spawnRayCast) {
    sf::Vector2f origen = jugador.getPosicion();
    sf::Vector2f direccion;
    direccion.x = mira.getPosicion().x - origen.x;
    direccion.y = mira.getPosicion().y - origen.y;
    
    float longitud = std::sqrt(direccion.x*direccion.x + direccion.y * direccion.y);
    
    direccion.x /= longitud;
    direccion.y /= longitud;

    float angulo = std::atan2(direccion.y, direccion.x)* 180.f / 3.14159f;
    trazaMosin.setPosition(origen.x,origen.y+10);
    trazaMosin.setRotation(angulo);

    mostrarTrazaMosin = true;
    tiempoTrazaMosin = 0.05f;
    float distanciaImpacto;
    float alcance = jugador.getArma().getAlcance();
    
    for (float distancia = 0.f; distancia < alcance; distancia += 5.f) {
      sf::Vector2f punto;
      
      punto.x = origen.x + direccion.x * distancia;
      punto.y = origen.y + direccion.y * distancia;
      trazaMosin.setSize(sf::Vector2f(alcance, 3.f));
      for(auto& obstaculo : obstaculos) {
        if (obstaculo.getHitbox().contains(punto)) {
          distanciaImpacto = distancia;
          trazaMosin.setSize(sf::Vector2f(distanciaImpacto, 3.f));
          return;
        }
      }
      for(auto& zombie : zombieManager.getZombies()) {
        if(zombie.getHitbox().contains(punto)) {
          zombie.recibirDanio(jugador.getArma().getDanio());
        }
      }
    }
  }
}
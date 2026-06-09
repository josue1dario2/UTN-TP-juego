#pragma once
#include "ObjetoMapa.h"
#include "Personaje.h"
#include "Puntero.h"
#include "Zombie.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Juego {
private:
  // VENTANA Y VISTA
  sf::RenderWindow ventana;
  sf::View vista;

  float auxVistaX;
  float auxVistaY;

  // RELOJ PARA CONTROLAR EL TIEMPO ENTRE FRAMES
  sf::Clock relojDelta;
  float deltaTime;

  // INCIALIZACION DE ELEMENTOS DEL JUEGO
  Personaje jugador;

  std::vector<ObjetoMapa> obstaculos; // Vector para almacenar múltiples
                                      // elementos del mapa/paredes/obstaculos
  std::vector<Zombie>
      zombies; // Vector para almacenar a los zombies activos en la partida

  std::vector<sf::FloatRect> zonasSpawn;
  int indiceZonaActiva;
  sf::Texture texturaMapa;
  sf::Sprite spriteMapa;

  Puntero mira;

  void procesarEventos();
  void actualizar();
  void renderizar();

  void inicializarObstaculos(std::vector<ObjetoMapa> &obstaculos);
  void inicializarZombies();

public:
  Juego();
  void iniciar();
};
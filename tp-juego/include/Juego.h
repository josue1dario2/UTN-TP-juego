#pragma once
#include "ObjetoMapa.h"
#include "Personaje.h"
#include "Puntero.h"
#include "ZombieManager.h"
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

  std::vector<ObjetoMapa> obstaculos; // Vector para almacenar múltiples elementos del mapa/paredes/obstaculos
  std::vector<Proyectil> proyectiles; // Vector para almacenar múltiples proyectiles
  
  ZombieManager zombieManager;

  sf::Texture texturaMapa;
  sf::Sprite spriteMapa;
  sf::Texture texturaProyectil;

  Puntero mira;

  void procesarEventos();
  void actualizar();
  void renderizar();

  void inicializarObstaculos(std::vector<ObjetoMapa> &obstaculos);

public:
  Juego(int idJug, int idArma, std::string nombre, float vida, float armadura, float velocidad, float cooldown);
  void iniciar();
};
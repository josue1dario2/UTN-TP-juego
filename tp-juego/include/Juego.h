#pragma once
#include "ObjetoMapa.h"
#include "Personaje.h"
#include "Puntero.h"
#include "ZombieManager.h"
#include "Hud.h"
#include "Mina.h"
#include "Boton.h"
#include "archivoPersonaje.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class EstadoJuego {
    MenuPrincipal,
    Estadisticas,
    SeleccionPersonaje,
    Jugando,
    GameOver
};


class Estadistica {
private:
    int _partidasJugadas;
    int _oleadaMaxima;
    int _zombiesEliminados;

public:
    Estadistica() : _partidasJugadas(0), _oleadaMaxima(0), _zombiesEliminados(0) {}

    int getPartidasJugadas() const { return _partidasJugadas; }
    void setPartidasJugadas(int valor) { _partidasJugadas = valor; }

    int getOleadaMaxima() const { return _oleadaMaxima; }
    void setOleadaMaxima(int valor) { _oleadaMaxima = valor; }

    int getZombiesEliminados() const { return _zombiesEliminados; }
    void setZombiesEliminados(int valor) { _zombiesEliminados = valor; }

    void registrarNuevaPartida() { _partidasJugadas++; }
    
    void registrarOleadaMaxima(int oleada) {
        if (oleada > _oleadaMaxima) {
            _oleadaMaxima = oleada;
        }
    }

    void sumarZombiesEliminados(int cantidad) { _zombiesEliminados += cantidad; }
};

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
  std::vector<Mina> trampas;
  
  ZombieManager zombieManager;
  Hud hud;

  sf::Texture texturaMapa;
  sf::Sprite spriteMapa;
  sf::Texture texturaProyectil;

  Puntero mira;

  // ELEMENTOS DE MENÚ Y MÁQUINA DE ESTADOS
  EstadoJuego estadoActual;
  sf::Font fuenteMenu;
  sf::Text tituloJuego;
  
  // Fondo personalizado
  sf::Texture texturaFondoMenu;
  sf::Sprite spriteFondoMenu;
  bool tieneFondoMenu;
  
  // Botones menú principal
  Boton btnMenuJugar;
  Boton btnMenuStats;
  Boton btnMenuSalir;
  

  
  // Selección personaje
  struct BotonPersonaje {
      Boton boton;
      RegistroPersonaje registro;
      sf::Texture textura;
      sf::Sprite sprite;
  };
  std::vector<BotonPersonaje> botonesPersonajes;
  Boton btnVolverSeleccion;
  
  RegistroPersonaje personajeSeleccionado;

  // Estadísticas
  Estadistica statsHistoricas;
  sf::Text textoStats;
  Boton btnVolverStats;
  
  int indiceMenuSeleccionado;

  void procesarEventos();
  void actualizar();
  void renderizar();

  void inicializarObstaculos(std::vector<ObjetoMapa> &obstaculos);
  void procesarRayCast();
  
  sf::RectangleShape trazaMosin;
  bool mostrarTrazaMosin = false;
  float tiempoTrazaMosin = 0.f;

  // Métodos del Menú
  void inicializarMenus();
  void actualizarMenu(sf::Vector2f posMouse);
  void renderizarMenu();
  
  void actualizarEstadisticas(sf::Vector2f posMouse);
  void renderizarEstadisticas();

  void actualizarSeleccionPersonaje(sf::Vector2f posMouse);
  void renderizarSeleccionPersonaje();
  void iniciarPartidaDirecta();
  
  // Guardar y Cargar Estadísticas
  void guardarStats();
  void cargarStats();

public:
  Juego();
  void iniciar();
};
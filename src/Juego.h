#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

// Incluimos los módulos que usarán composición directa
#include "Oleada.h"
#include "HUD.h"
#include "Menu.h"
#include "GestorRecursos.h"
#include "GestorDeArchivos.h"
#include "Mapa.h"
#include "Personaje.h"
#include "ArmaDatos.h"
#include "Partida.h"

// Forward declarations para punteros
class Jugador;
class Enemigo;
class Bala;
class Arma;

enum class GameState {
    MENU_PRINCIPAL,
    SELECCION_PERSONAJE,
    JUGANDO,
    PAUSADO,
    GAME_OVER,
    HIGH_SCORES
};

class Juego {
public:
    Juego();
    ~Juego();
    void ejecutar();

    // Metodos para guardar/cargar partida
    void guardarPartida();
    Partida cargarPartida();

private:
    void procesarEventos();
    void actualizar(float deltaTime);
    void renderizar();
    
    void cargarRecursos();
    void cargarDatosJuego();
    void inicializarJugador(int indicePersonaje);
    void darArmasJugador();
    
    void spawnearZombie();
    void verificarColisiones();
    void limpiarEntidadesMuertas();
    void limpiar();
    void reiniciarJuego();
    void actualizarJugando(float deltaTime);
    void manejarResultadoMenu(MenuResult resultado);
    void dibujarSuelo();

    sf::RenderWindow ventana;
    sf::Clock reloj;

    GameState estadoActual;
    float tiempoSupervivencia;
    bool transicionOleada;
    float timerTransicionOleada;

    Jugador* jugador;
    std::vector<Enemigo*> zombies;
    std::vector<Bala*> balas;

    Oleada gestorOleadas;
    HUD interfazHUD;
    Menu menu;
    GestorRecursos recursos;
    Mapa mapa;
    sf::Vector2f camara;

    std::vector<Personaje> listaPersonajes;
    std::vector<ArmaDatos> listaArmas;

    sf::Texture texturaRespaldoJugador;
    sf::Texture texturaRespaldoZombie;
    sf::Texture texturaRespaldoBala;
    sf::Texture texturaRespaldoZombieRapido;
    sf::Texture texturaRespaldoZombiePesado;
    sf::Font fuenteRespaldo;

    float enfriamientoDanoZombie;
    sf::RectangleShape figuraSuelo;
    sf::Texture texturaSuelo;
};
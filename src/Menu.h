#pragma once

// ============================================================================
// Menu.h - Sistema de menús del juego
// ============================================================================
// Este módulo maneja todas las pantallas de menú:
//   1. Menú Principal: JUGAR, RECORDS, SALIR
//   2. Selección de Personaje: elegir entre los personajes disponibles
//   3. Game Over: muestra puntuación final y opciones de reintentar
//   4. Tabla de Records: muestra las mejores puntuaciones
//
// Usa un enum class MenuResult para comunicar la acción elegida por el jugador.
// El enum class es más seguro que un enum normal porque tiene scope propio.
// ============================================================================

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Personaje.h"
#include "Estadistica.h"

// ============================================================================
// Enum de resultados del menú
// ============================================================================
// Cada valor representa una acción que el jugador puede realizar.
// NONE significa que no se ha tomado ninguna decisión todavía.
enum class MenuResult {
    NONE,                 // Sin acción (el jugador aún está navegando)
    PLAY,                 // Ir a selección de personaje / iniciar juego
    HIGH_SCORES,          // Ver tabla de récords
    QUIT,                 // Salir del juego
    BACK,                 // Volver al menú anterior
    RETRY,                // Reintentar la partida
    CHARACTER_SELECTED    // Se ha seleccionado un personaje
};

// ============================================================================
// Enum de estados del menú
// ============================================================================
// Indica qué pantalla de menú se está mostrando actualmente.
// Se usa externamente (en Game.cpp) para saber qué menú renderizar.
enum class MenuState {
    MENU_PRINCIPAL,            // Menú principal
    SELECCION_PERSONAJE,     // Selección de personaje
    GAME_OVER,            // Pantalla de fin de partida
    HIGH_SCORES           // Tabla de récords
};

class Menu {
public:
    // Constructor por defecto
    Menu();

    // Inicializa todos los elementos visuales del menú.
    // Recibe la fuente y los datos de personajes cargados desde archivo.
    void inicializar(sf::Font& font, const std::vector<Personaje>& personajes);

    // Procesa la entrada del jugador (teclado) según el estado actual del menú.
    // Retorna MenuResult indicando la acción elegida.
    MenuResult procesarEntrada(const sf::Event& event, const sf::RenderWindow& window);

    // Dibuja el menú correspondiente al estado actual.
    void dibujar(sf::RenderWindow& window, MenuState estado);

    // Almacena los datos finales para mostrar en la pantalla de Game Over.
    void establecerDatosGameOver(int puntuacion, int oleada, float tiempo);

    // Almacena las estadísticas para mostrar en la tabla de récords.
    void establecerHighScores(const std::vector<Estadistica>& estadisticas);

    // Retorna el índice del personaje seleccionado (0, 1, 2...).
    int getPersonajeSeleccionado() const;

    // Establece el estado actual del menú (qué pantalla mostrar).
    void setMenuState(MenuState estado);

    // Obtiene el estado actual del menú.
    MenuState getMenuState() const;

private:
    // ---- Estado del menú ----
    MenuState estadoActual;    // Qué pantalla se está mostrando

    // ---- Puntero a la fuente ----
    sf::Font* font;

    // ---- Menú Principal ----
    sf::Text titulo;            // "Z-WAVE" en grande
    sf::Text subtitulo;         // "Supervivencia Zombie"
    static const int CANTIDAD_OPCIONES_MENU = 3;
    sf::Text opcionesMenu[CANTIDAD_OPCIONES_MENU]; // "JUGAR", "RECORDS", "SALIR"
    int opcionMenuSeleccionada;    // Índice de la opción seleccionada (0-2)

    // ---- Selección de Personaje ----
    std::vector<Personaje> datosPersonajes; // Datos cargados de archivo
    std::vector<sf::Text> nombresPersonajes;          // Nombres de personajes
    std::vector<sf::Text> statsPersonajes;          // Estadísticas de cada personaje
    std::vector<sf::Sprite> spritesPersonajes;     // Sprites de cada personaje para selection menu
    std::vector<sf::Texture> texturasPersonajes;   // Texturas propias por personaje (evita que se invaliden al salir del scope)
    sf::Text tituloSeleccion;                     // "ELIGE TU PERSONAJE"
    sf::Text instruccionesSeleccion;              // "← → para elegir, ENTER para confirmar"
    int personajeSeleccionado;                    // Índice del personaje seleccionado
    float cardWidth;                          // Ancho de tarjeta para seleccion
    float cardHeight;                         // Alto de tarjeta para seleccion

    // ---- Pantalla de Game Over ----
    sf::Text tituloGameOver;    // "GAME OVER"
    sf::Text textoPuntuacionFinal;   // "PUNTUACIÓN: 5000"
    sf::Text textoOleadaFinal;    // "OLEADA ALCANZADA: 7"
    sf::Text textoTiempoFinal;    // "TIEMPO: 05:30"
    sf::Text opcionReintentar;      // "REINTENTAR"
    sf::Text opcionMenu;       // "MENÚ PRINCIPAL"
    int opcionGameOverSeleccionada;// 0 = reintentar, 1 = menú

    // ---- Tabla de Récords ----
    sf::Text tituloScores;               // "MEJORES PUNTUACIONES"
    std::vector<sf::Text> entradasScores; // Lista de puntuaciones
    sf::Text textoVolver;                  // "PRESIONA ESC PARA VOLVER"

    // ---- Efecto visual del título ----
    float timerPulsoTitulo;     // Temporizador para efecto de pulso en el título

    // ---- Métodos privados de renderizado ----
    void dibujarMenuPrincipal(sf::RenderWindow& window);
    void dibujarSeleccionPersonaje(sf::RenderWindow& window);
    void dibujarGameOver(sf::RenderWindow& window);
    void dibujarHighScores(sf::RenderWindow& window);

    // ---- Métodos privados de entrada ----
    MenuResult manejarEntradaMenuPrincipal(const sf::Event& event);
    MenuResult manejarEntradaSeleccionPersonaje(const sf::Event& event);
    MenuResult manejarEntradaGameOver(const sf::Event& event);
    MenuResult manejarEntradaHighScores(const sf::Event& event);

    // ---- Utilidades ----
    // Centra un texto horizontalmente en la ventana (ancho 1280)
    void centrarTextoHorizontalmente(sf::Text& text, float yPosition);

    // Formatea segundos a "MM:SS"
    std::string formatearTiempo(float seconds) const;
};

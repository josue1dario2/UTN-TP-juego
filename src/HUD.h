#pragma once

// ============================================================================
// HUD.h - Heads-Up Display (Interfaz en pantalla durante el juego)
// ============================================================================
// El HUD muestra información vital al jugador durante la partida:
//   - Barra de vida y armadura (con barras visuales de colores)
//   - Oleada actual, dinero, puntuación
//   - Munición del arma equipada
//   - Tiempo de supervivencia
//   - Anuncios de oleada ("¡OLEADA 3!")
//
// El HUD se dibuja encima de todo el contenido del juego usando posiciones
// fijas en la ventana (no se mueve con la cámara del juego).
// ============================================================================

#include <SFML/Graphics.hpp>
#include <string>

class HUD {
public:
    // Constructor por defecto
    HUD();

    // Inicializa todos los elementos del HUD con la fuente proporcionada.
    // Se debe llamar una vez después de cargar la fuente del juego.
    void inicializar(sf::Font& font);

    // Actualiza todos los valores mostrados en el HUD.
    // Se llama cada frame con los datos actuales del juego.
    void update(int health, int maxHealth, int armor, int maxArmor,
                int wave, int money, int score,
                int ammoInMag, int totalAmmo,
                const std::string& weaponName,
                float survivalTime, float deltaTime);

    // Muestra un anuncio grande en el centro de la pantalla para una nueva oleada.
    void showWaveAnnouncement(int waveNum);

    // Dibuja todos los elementos del HUD en la ventana.
    void render(sf::RenderWindow& window);

private:
    // ---- Referencia a la fuente ----
    // Puntero a la fuente (no somos dueños de la fuente, solo la referenciamos)
    sf::Font* font;

    // ---- Elementos de texto ----
    // sf::Text es el objeto de SFML para renderizar texto en pantalla
    sf::Text healthText;           // Muestra "VIDA: 100/150"
    sf::Text armorText;            // Muestra "ARMADURA: 50/100"
    sf::Text waveText;             // Muestra "OLEADA: 3"
    sf::Text moneyText;            // Muestra "$ 500"
    sf::Text scoreText;            // Muestra "PUNTOS: 1500"
    sf::Text ammoText;             // Muestra "12 / 60"
    sf::Text weaponText;           // Muestra nombre del arma equipada
    sf::Text timeText;             // Muestra "TIEMPO: 05:30"
    sf::Text waveAnnouncementText; // Texto grande "¡OLEADA 3!"

    // ---- Barras de vida y armadura ----
    // Cada barra tiene un fondo oscuro (bg) y una barra de color encima
    sf::RectangleShape healthBar;     // Barra roja de vida
    sf::RectangleShape healthBarBg;   // Fondo oscuro de la barra de vida
    sf::RectangleShape armorBar;      // Barra azul de armadura
    sf::RectangleShape armorBarBg;    // Fondo oscuro de la barra de armadura

    // ---- Paneles de fondo ----
    // Rectángulos semi-transparentes para mejorar la legibilidad del texto
    sf::RectangleShape topLeftPanel;    // Panel detrás de vida/armadura
    sf::RectangleShape topRightPanel;   // Panel detrás de dinero/puntos
    sf::RectangleShape bottomRightPanel;// Panel detrás de munición/arma
    sf::RectangleShape topCenterPanel;  // Panel detrás de oleada/tiempo

    // ---- Temporizador del anuncio ----
    // Controla cuánto tiempo se muestra el anuncio de oleada
    float announcementTimer;

    // ---- Constantes de diseño ----
    // Dimensiones máximas de las barras de vida/armadura
    static constexpr float BAR_WIDTH = 200.0f;
    static constexpr float BAR_HEIGHT = 20.0f;
    static constexpr float ANNOUNCEMENT_DURATION = 3.0f; // Segundos que dura el anuncio

    // ---- Método auxiliar ----
    // Formatea un tiempo en segundos como "MM:SS"
    std::string formatTime(float seconds) const;
};

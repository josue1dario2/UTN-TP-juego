// ============================================================================
// HUD.cpp - Implementación del Heads-Up Display
// ============================================================================
// El HUD se compone de tres áreas principales:
//   1. Esquina superior izquierda: barras de vida y armadura
//   2. Esquina superior derecha: dinero, puntuación
//   3. Esquina inferior derecha: munición y nombre del arma
//   4. Centro superior: oleada actual y tiempo de supervivencia
//   5. Centro de pantalla: anuncios de oleada (temporales)
//
// Todas las posiciones son relativas a la ventana de 1280x720.
// ============================================================================

#include "HUD.h"
#include <sstream>  // Para std::ostringstream (formateo de strings)
#include <iomanip>  // Para std::setfill y std::setw (formato de tiempo)

// ============================================================================
// Constructor - Inicializa valores por defecto
// ============================================================================
HUD::HUD()
    : font(nullptr)
    , announcementTimer(0.0f)
{
    // Los demás objetos (sf::Text, sf::RectangleShape) se inicializan
    // automáticamente con sus constructores por defecto
}

// ============================================================================
// init - Configurar todos los elementos visuales del HUD
// ============================================================================
// Se llama una sola vez al inicio del juego. Configura fuentes, posiciones,
// tamaños y colores de todos los elementos.
void HUD::inicializar(sf::Font& fontRef) {
    // Guardar puntero a la fuente para uso interno
    font = &fontRef;

    // ---- Colores del HUD ----
    // Definimos los colores como variables locales para reutilizarlos
    sf::Color colorVida(220, 50, 50);          // Rojo para vida
    sf::Color colorArmadura(50, 120, 220);     // Azul para armadura
    sf::Color colorDinero(255, 215, 0);        // Dorado para dinero
    sf::Color colorTexto(200, 200, 200);       // Gris claro para texto general
    sf::Color colorFondoPanel(0, 0, 0, 150);   // Negro semi-transparente para paneles
    sf::Color colorFondoBarra(50, 50, 50);     // Gris oscuro para fondo de barras

    // ========================================================================
    // ÁREA SUPERIOR IZQUIERDA - Vida y Armadura
    // ========================================================================

    // -- Panel de fondo (mejora legibilidad sobre el mapa) --
    topLeftPanel.setSize(sf::Vector2f(230.0f, 100.0f));
    topLeftPanel.setPosition(5.0f, 5.0f);
    topLeftPanel.setFillColor(colorFondoPanel);

    // -- Texto de vida --
    healthText.setFont(*font);
    healthText.setCharacterSize(16);
    healthText.setFillColor(colorVida);
    healthText.setPosition(15.0f, 10.0f);
    healthText.setString("VIDA: 100/100");

    // -- Barra de vida: fondo oscuro --
    healthBarBg.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
    healthBarBg.setPosition(15.0f, 32.0f);
    healthBarBg.setFillColor(colorFondoBarra);
    healthBarBg.setOutlineColor(sf::Color(100, 100, 100));
    healthBarBg.setOutlineThickness(1.0f);

    // -- Barra de vida: barra roja (se redimensiona según la vida actual) --
    healthBar.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
    healthBar.setPosition(15.0f, 32.0f);
    healthBar.setFillColor(colorVida);

    // -- Texto de armadura --
    armorText.setFont(*font);
    armorText.setCharacterSize(16);
    armorText.setFillColor(colorArmadura);
    armorText.setPosition(15.0f, 58.0f);
    armorText.setString("ARMADURA: 50/100");

    // -- Barra de armadura: fondo oscuro --
    armorBarBg.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
    armorBarBg.setPosition(15.0f, 80.0f);
    armorBarBg.setFillColor(colorFondoBarra);
    armorBarBg.setOutlineColor(sf::Color(100, 100, 100));
    armorBarBg.setOutlineThickness(1.0f);

    // -- Barra de armadura: barra azul --
    armorBar.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
    armorBar.setPosition(15.0f, 80.0f);
    armorBar.setFillColor(colorArmadura);

    // ========================================================================
    // ÁREA SUPERIOR CENTRAL - Oleada y Tiempo
    // ========================================================================

    topCenterPanel.setSize(sf::Vector2f(220.0f, 55.0f));
    topCenterPanel.setPosition(530.0f, 5.0f);
    topCenterPanel.setFillColor(colorFondoPanel);

    // -- Texto de oleada actual --
    waveText.setFont(*font);
    waveText.setCharacterSize(18);
    waveText.setFillColor(colorTexto);
    waveText.setPosition(545.0f, 8.0f);
    waveText.setString("OLEADA: 1");

    // -- Texto de tiempo de supervivencia --
    timeText.setFont(*font);
    timeText.setCharacterSize(16);
    timeText.setFillColor(colorTexto);
    timeText.setPosition(545.0f, 33.0f);
    timeText.setString("TIEMPO: 00:00");

    // ========================================================================
    // ÁREA SUPERIOR DERECHA - Dinero y Puntuación
    // ========================================================================

    topRightPanel.setSize(sf::Vector2f(180.0f, 55.0f));
    topRightPanel.setPosition(1090.0f, 5.0f);
    topRightPanel.setFillColor(colorFondoPanel);

    // -- Texto de puntos (scoreText solo, sin moneyText redundante) --
    scoreText.setFont(*font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(colorDinero);
    scoreText.setPosition(1100.0f, 15.0f);
    scoreText.setString("PUNTOS: 0");

    // ========================================================================
    // ÁREA INFERIOR DERECHA - Arma y Munición
    // ========================================================================

    bottomRightPanel.setSize(sf::Vector2f(180.0f, 55.0f));
    bottomRightPanel.setPosition(1090.0f, 660.0f);
    bottomRightPanel.setFillColor(colorFondoPanel);

    // -- Nombre del arma equipada --
    weaponText.setFont(*font);
    weaponText.setCharacterSize(16);
    weaponText.setFillColor(colorTexto);
    weaponText.setPosition(1100.0f, 663.0f);
    weaponText.setString("Pistola");

    // -- Texto de munición (balas en cargador / total) --
    ammoText.setFont(*font);
    ammoText.setCharacterSize(18);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setPosition(1100.0f, 688.0f);
    ammoText.setString("12 / 60");

    // ========================================================================
    // ANUNCIO DE OLEADA - Centro de pantalla (temporal)
    // ========================================================================

    waveAnnouncementText.setFont(*font);
    waveAnnouncementText.setCharacterSize(48);
    waveAnnouncementText.setFillColor(sf::Color(220, 50, 50));
    waveAnnouncementText.setOutlineColor(sf::Color::Black);
    waveAnnouncementText.setOutlineThickness(3.0f);
    waveAnnouncementText.setStyle(sf::Text::Bold);
    waveAnnouncementText.setString("");
}

// ============================================================================
// update - Actualizar todos los valores del HUD cada frame
// ============================================================================
// Recibe los datos actuales del juego y actualiza los textos y barras.
void HUD::update(int health, int maxHealth, int armor, int maxArmor,
                 int wave, int money, int score,
                 int ammoInMag, int totalAmmo,
                 const std::string& weaponName,
                 float survivalTime, float deltaTime) {

    // ---- Actualizar texto de vida ----
    // std::ostringstream permite construir strings formateados fácilmente
    std::ostringstream healthSS;
    healthSS << "VIDA: " << health << "/" << maxHealth;
    healthText.setString(healthSS.str());

    // ---- Actualizar barra de vida ----
    // Calcular el ancho proporcional: (vida actual / vida máxima) * ancho máximo
    // Evitar división por cero con el operador ternario
    float healthRatio = (maxHealth > 0) ? static_cast<float>(health) / maxHealth : 0.0f;
    // Clamp entre 0 y 1 para evitar barras negativas o más grandes que el máximo
    if (healthRatio < 0.0f) healthRatio = 0.0f;
    if (healthRatio > 1.0f) healthRatio = 1.0f;
    healthBar.setSize(sf::Vector2f(BAR_WIDTH * healthRatio, BAR_HEIGHT));

    // Cambiar color de la barra según el nivel de vida
    // Rojo brillante si está bien, naranja si baja, rojo oscuro si crítico
    if (healthRatio > 0.5f) {
        healthBar.setFillColor(sf::Color(220, 50, 50));     // Rojo normal
    } else if (healthRatio > 0.25f) {
        healthBar.setFillColor(sf::Color(220, 140, 50));    // Naranja (¡cuidado!)
    } else {
        healthBar.setFillColor(sf::Color(180, 30, 30));     // Rojo oscuro (¡peligro!)
    }

    // ---- Actualizar texto de armadura ----
    std::ostringstream armorSS;
    armorSS << "ARMADURA: " << armor << "/" << maxArmor;
    armorText.setString(armorSS.str());

    // ---- Actualizar barra de armadura ----
    float armorRatio = (maxArmor > 0) ? static_cast<float>(armor) / maxArmor : 0.0f;
    if (armorRatio < 0.0f) armorRatio = 0.0f;
    if (armorRatio > 1.0f) armorRatio = 1.0f;
    armorBar.setSize(sf::Vector2f(BAR_WIDTH * armorRatio, BAR_HEIGHT));

    // ---- Actualizar texto de oleada ----
    std::ostringstream waveSS;
    waveSS << "OLEADA: " << wave;
    waveText.setString(waveSS.str());

    std::ostringstream scoreSS;
    scoreSS << "PUNTOS: " << score;
    scoreText.setString(scoreSS.str());

    // ---- Actualizar nombre del arma ----
    weaponText.setString(weaponName);

    // ---- Actualizar texto de munición ----
    std::ostringstream ammoSS;
    ammoSS << ammoInMag << " / " << totalAmmo;
    ammoText.setString(ammoSS.str());

    // Cambiar color según la munición restante en el cargador
    if (ammoInMag == 0) {
        ammoText.setFillColor(sf::Color(220, 50, 50)); // Rojo si no hay balas
    } else if (ammoInMag <= 3) {
        ammoText.setFillColor(sf::Color(220, 180, 50)); // Amarillo si quedan pocas
    } else {
        ammoText.setFillColor(sf::Color::White);        // Blanco normal
    }

    // ---- Actualizar tiempo de supervivencia ----
    std::ostringstream timeSS;
    timeSS << "TIEMPO: " << formatTime(survivalTime);
    timeText.setString(timeSS.str());

    // ---- Actualizar temporizador del anuncio de oleada ----
    // El anuncio se desvanece con el tiempo
    if (announcementTimer > 0.0f) {
        announcementTimer -= deltaTime;

        // Calcular opacidad para efecto de desvanecimiento (fade out)
        // Los últimos 1.5 segundos se hace fade out
        float alpha = 255.0f;
        if (announcementTimer < 1.5f) {
            alpha = (announcementTimer / 1.5f) * 255.0f;
        }

        // Aplicar la opacidad al color del texto
        sf::Color announcementColor = waveAnnouncementText.getFillColor();
        announcementColor.a = static_cast<sf::Uint8>(alpha);
        waveAnnouncementText.setFillColor(announcementColor);

        // También al contorno
        sf::Color outlineColor = waveAnnouncementText.getOutlineColor();
        outlineColor.a = static_cast<sf::Uint8>(alpha);
        waveAnnouncementText.setOutlineColor(outlineColor);
    }
}

// ============================================================================
// showWaveAnnouncement - Mostrar anuncio de nueva oleada
// ============================================================================
// Muestra un texto grande en el centro de la pantalla indicando la oleada.
void HUD::showWaveAnnouncement(int waveNum) {
    // Construir el texto del anuncio
    std::ostringstream ss;
    ss << "OLEADA " << waveNum;
    waveAnnouncementText.setString(ss.str());

    // Centrar el texto en la pantalla (1280x720)
    // Para centrar, calculamos el ancho del texto y restamos la mitad
    sf::FloatRect bounds = waveAnnouncementText.getLocalBounds();
    waveAnnouncementText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    waveAnnouncementText.setPosition(640.0f, 300.0f); // Centro de 1280x720

    // Resetear colores a opacidad completa
    waveAnnouncementText.setFillColor(sf::Color(220, 50, 50, 255));
    waveAnnouncementText.setOutlineColor(sf::Color(0, 0, 0, 255));

    // Iniciar el temporizador
    announcementTimer = ANNOUNCEMENT_DURATION;
}

// ============================================================================
// render - Dibujar todos los elementos del HUD
// ============================================================================
// El orden de dibujo importa: primero los paneles de fondo, luego las barras,
// luego los textos encima.
void HUD::render(sf::RenderWindow& window) {
    // ---- Dibujar paneles de fondo semi-transparentes ----
    // Estos paneles oscurecen el área detrás del texto para que sea legible
    // independientemente del color del mapa debajo
    window.draw(topLeftPanel);
    window.draw(topCenterPanel);
    window.draw(topRightPanel);
    window.draw(bottomRightPanel);

    // ---- Dibujar barras de vida ----
    // Primero el fondo (gris oscuro), luego la barra de color encima
    window.draw(healthBarBg);
    window.draw(healthBar);

    // ---- Dibujar barras de armadura ----
    window.draw(armorBarBg);
    window.draw(armorBar);

    // ---- Dibujar todos los textos ----
    window.draw(healthText);
    window.draw(armorText);
    window.draw(waveText);
    window.draw(timeText);
    window.draw(scoreText);
    window.draw(weaponText);
    window.draw(ammoText);

    // ---- Dibujar anuncio de oleada si está activo ----
    if (announcementTimer > 0.0f) {
        window.draw(waveAnnouncementText);
    }
}

// ============================================================================
// formatTime - Convertir segundos a formato MM:SS
// ============================================================================
// Ejemplo: 125.7 segundos -> "02:05"
std::string HUD::formatTime(float seconds) const {
    // Convertir a entero para calcular minutos y segundos
    int totalSeconds = static_cast<int>(seconds);
    int minutes = totalSeconds / 60;    // División entera da los minutos
    int secs = totalSeconds % 60;       // Módulo da los segundos restantes

    // Formatear con ceros a la izquierda usando std::setfill y std::setw
    // setfill('0') rellena con ceros, setw(2) asegura al menos 2 dígitos
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes
       << ":"
       << std::setfill('0') << std::setw(2) << secs;

    return ss.str();
}

// ============================================================================
// Menu.cpp - Implementación del sistema de menús
// ============================================================================
// Este archivo implementa toda la lógica de navegación e interacción de los
// menús del juego. Cada estado del menú tiene su propio método de renderizado
// y de manejo de entrada, lo que mantiene el código organizado y modular.
//
// Flujo de navegación:
//   Menú Principal → Selección de Personaje → Juego
//   Menú Principal → Récords → Menú Principal
//   Game Over → Reintentar / Menú Principal
// ============================================================================

#include "Menu.h"
#include <sstream>
#include <iomanip>
#include <algorithm> // Para std::sort
#include <cmath>     // Para std::sin (efecto de pulso)

// ============================================================================
// Constructor - Valores por defecto
// ============================================================================
Menu::Menu()
    : estadoActual(MenuState::MENU_PRINCIPAL)
    , font(nullptr)
    , opcionMenuSeleccionada(0)
    , personajeSeleccionado(0)
    , opcionGameOverSeleccionada(0)
    , timerPulsoTitulo(0.0f)
{
}

// ============================================================================
// init - Inicializar todos los elementos visuales
// ============================================================================
// Configura fuentes, posiciones, tamaños y colores de todos los textos.
// Se llama una sola vez al inicio del juego.
void Menu::inicializar(sf::Font& fontRef, const std::vector<Personaje>& personajes) {
    font = &fontRef;
    datosPersonajes = personajes;

    // ---- Colores reutilizables ----
    sf::Color colorTitulo(220, 50, 50);      // Rojo sangre para el título
    sf::Color colorSubtitulo(180, 180, 180); // Gris claro para subtítulo
    sf::Color colorNormal(200, 200, 200);    // Gris para opciones no seleccionadas
    sf::Color colorSeleccionado(255, 255, 100); // Amarillo para opción seleccionada
    sf::Color colorDorado(255, 215, 0);      // Dorado para puntuaciones

    // ========================================================================
    // MENÚ PRINCIPAL
    // ========================================================================

    // -- Título principal "Z-WAVE" --
    titulo.setFont(*font);
    titulo.setCharacterSize(72);
    titulo.setFillColor(colorTitulo);
    titulo.setOutlineColor(sf::Color::Black);
    titulo.setOutlineThickness(3.0f);
    titulo.setStyle(sf::Text::Bold);
    titulo.setString("Z-WAVE");
    centrarTextoHorizontalmente(titulo, 120.0f);

    // -- Subtítulo "Supervivencia Zombie" --
    subtitulo.setFont(*font);
    subtitulo.setCharacterSize(24);
    subtitulo.setFillColor(colorSubtitulo);
    subtitulo.setString("Supervivencia Zombie");
    centrarTextoHorizontalmente(subtitulo, 210.0f);

    // -- Opciones del menú principal --
    std::string etiquetasOpciones[CANTIDAD_OPCIONES_MENU] = {"JUGAR", "RECORDS", "SALIR"};
    for (int i = 0; i < CANTIDAD_OPCIONES_MENU; i++) {
        opcionesMenu[i].setFont(*font);
        opcionesMenu[i].setCharacterSize(28);
        opcionesMenu[i].setString(etiquetasOpciones[i]);
        centrarTextoHorizontalmente(opcionesMenu[i], 350.0f + i * 60.0f);
    }

    // ========================================================================
    // SELECCIÓN DE PERSONAJE
    // ========================================================================

    // -- Título de la pantalla de selección --
    tituloSeleccion.setFont(*font);
    tituloSeleccion.setCharacterSize(36);
    tituloSeleccion.setFillColor(colorTitulo);
    tituloSeleccion.setStyle(sf::Text::Bold);
    tituloSeleccion.setString("ELIGE TU PERSONAJE");
    centrarTextoHorizontalmente(tituloSeleccion, 50.0f);

    // -- Instrucciones de navegación --
    instruccionesSeleccion.setFont(*font);
    instruccionesSeleccion.setCharacterSize(16);
    instruccionesSeleccion.setFillColor(sf::Color(150, 150, 150));
    instruccionesSeleccion.setString("Flechas IZQUIERDA/DERECHA para elegir  |  ENTER para confirmar  |  ESC para volver");
    centrarTextoHorizontalmente(instruccionesSeleccion, 660.0f);

    // -- Crear textos para cada personaje --
    nombresPersonajes.clear();
    statsPersonajes.clear();

    // -- Layout: 5 personajes distribuidos en 2 filas --
    // Fila 1: 3 personajes (Joel, Sniper, Ghost) - indices 0,1,2
    // Fila 2: 2 personajes (John Wick, Soldado) - indices 3,4
    float cardWidth = 350.0f;
    float cardHeight = 200.0f;
    float gapX = 60.0f;
    float gapY = 80.0f;
    float startY1 = 130.0f;
    float startY2 = startY1 + cardHeight + gapY;

    // Fila 1 centrada (3 tarjetas)
    float row1Width = 3 * cardWidth + 2 * gapX;
    float startX1 = (1280.0f - row1Width) / 2.0f;
    // Fila 2 centrada (2 tarjetas)
    float row2Width = 2 * cardWidth + 1 * gapX;
    float startX2 = (1280.0f - row2Width) / 2.0f;

    // Posiciones X para cada slot en cada fila
    float xRow1[3] = { startX1, startX1 + cardWidth + gapX, startX1 + 2 * (cardWidth + gapX) };
    float xRow2[2] = { startX2, startX2 + cardWidth + gapX };

    // Asignar posiciones por índice de personaje
    float posX[5], posY[5];
    if (personajes.size() >= 5) {
        posX[0] = xRow1[0]; posY[0] = startY1;
        posX[1] = xRow1[1]; posY[1] = startY1;
        posX[2] = xRow1[2]; posY[2] = startY1;
        posX[3] = xRow2[0]; posY[3] = startY2;
        posX[4] = xRow2[1]; posY[4] = startY2;
    }

    this->cardWidth = cardWidth;
    this->cardHeight = cardHeight;

    spritesPersonajes.clear();
    texturasPersonajes.clear();
    sf::Texture tex;
    std::vector<std::string> files = {
        "assets/textures/player_tank.png",
        "assets/textures/player_scout.png",
        "assets/textures/player_medic.png",
        "assets/textures/player_tank.png",
        "assets/textures/player_scout.png"
    };
    for (size_t i = 0; i < personajes.size() && i < files.size(); i++) {
        tex.loadFromFile(files[i]);
        texturasPersonajes.push_back(tex); // guardar textura propia para cada personaje
        sf::Sprite sp(texturasPersonajes.back());
        float scale = 120.0f / tex.getSize().x;
        sp.setScale(scale, scale);
        spritesPersonajes.push_back(sp);
    }

    for (size_t i = 0; i < personajes.size(); i++) {
        sf::Text nameText;
        nameText.setFont(*font);
        nameText.setCharacterSize(24);
        nameText.setStyle(sf::Text::Bold);
        nameText.setString(personajes[i].getNombre());
        nameText.setPosition(posX[i], posY[i]);
        nombresPersonajes.push_back(nameText);

        // -- partir habilidad en 2 lineas si es larga --
        std::string hab = personajes[i].getHabilidad();
        if (hab.length() > 14) {
            size_t mid = hab.find(' ', hab.length() / 2);
            if (mid != std::string::npos) {
                hab = hab.substr(0, mid) + "\n" + hab.substr(mid + 1);
            }
        }

        std::ostringstream statsSS;
        statsSS << "VIDA: " << personajes[i].getVidaInicial() << "\n"
                << "ARMADURA: " << personajes[i].getArmaduraInicial() << "\n"
                << "VELOCIDAD: " << personajes[i].getVelocidadInicial() << "\n"
                << "HABILIDAD:\n" << hab;

        sf::Text statsText;
        statsText.setFont(*font);
        statsText.setCharacterSize(15);
        statsText.setFillColor(colorNormal);
        statsText.setString(statsSS.str());
        statsText.setPosition(posX[i], posY[i] + 35.0f);
        statsPersonajes.push_back(statsText);
    }

    // ========================================================================
    // PANTALLA DE GAME OVER
    // ========================================================================

    // -- Título "GAME OVER" --
    tituloGameOver.setFont(*font);
    tituloGameOver.setCharacterSize(64);
    tituloGameOver.setFillColor(colorTitulo);
    tituloGameOver.setOutlineColor(sf::Color::Black);
    tituloGameOver.setOutlineThickness(3.0f);
    tituloGameOver.setStyle(sf::Text::Bold);
    tituloGameOver.setString("GAME OVER");
    centrarTextoHorizontalmente(tituloGameOver, 100.0f);

    // -- Texto de puntuación final --
    textoPuntuacionFinal.setFont(*font);
    textoPuntuacionFinal.setCharacterSize(24);
    textoPuntuacionFinal.setFillColor(colorDorado);
    textoPuntuacionFinal.setString("PUNTUACION: 0");
    centrarTextoHorizontalmente(textoPuntuacionFinal, 250.0f);

    // -- Texto de oleada alcanzada --
    textoOleadaFinal.setFont(*font);
    textoOleadaFinal.setCharacterSize(20);
    textoOleadaFinal.setFillColor(colorNormal);
    textoOleadaFinal.setString("OLEADA ALCANZADA: 0");
    centrarTextoHorizontalmente(textoOleadaFinal, 300.0f);

    // -- Texto de tiempo de supervivencia --
    textoTiempoFinal.setFont(*font);
    textoTiempoFinal.setCharacterSize(20);
    textoTiempoFinal.setFillColor(colorNormal);
    textoTiempoFinal.setString("TIEMPO: 00:00");
    centrarTextoHorizontalmente(textoTiempoFinal, 340.0f);

    // -- Opciones de Game Over --
    opcionReintentar.setFont(*font);
    opcionReintentar.setCharacterSize(28);
    opcionReintentar.setString("REINTENTAR");
    centrarTextoHorizontalmente(opcionReintentar, 450.0f);

    opcionMenu.setFont(*font);
    opcionMenu.setCharacterSize(28);
    opcionMenu.setString("MENU PRINCIPAL");
    centrarTextoHorizontalmente(opcionMenu, 510.0f);

    // ========================================================================
    // TABLA DE RÉCORDS
    // ========================================================================

    // -- Título --
    tituloScores.setFont(*font);
    tituloScores.setCharacterSize(36);
    tituloScores.setFillColor(colorDorado);
    tituloScores.setStyle(sf::Text::Bold);
    tituloScores.setString("MEJORES PUNTUACIONES");
    centrarTextoHorizontalmente(tituloScores, 60.0f);

    // -- Texto para volver --
    textoVolver.setFont(*font);
    textoVolver.setCharacterSize(16);
    textoVolver.setFillColor(sf::Color(150, 150, 150));
    textoVolver.setString("Presiona ESC para volver");
    centrarTextoHorizontalmente(textoVolver, 660.0f);
}

// ============================================================================
// procesarEntrada - Procesar entrada según el estado actual del menú
// ============================================================================
MenuResult Menu::procesarEntrada(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type != sf::Event::KeyPressed) {
        return MenuResult::NONE;
    }

    switch (estadoActual) {
        case MenuState::MENU_PRINCIPAL:
            return manejarEntradaMenuPrincipal(event);
        case MenuState::SELECCION_PERSONAJE:
            return manejarEntradaSeleccionPersonaje(event);
        case MenuState::GAME_OVER:
            return manejarEntradaGameOver(event);
        case MenuState::HIGH_SCORES:
            return manejarEntradaHighScores(event);
        default:
            return MenuResult::NONE;
    }
}

// ============================================================================
// Handlers de entrada específicos por estado
// ============================================================================

// ============================================================================
// manejarEntradaMenuPrincipal - Navegar y confirmar opciones del menú
// ============================================================================
MenuResult Menu::manejarEntradaMenuPrincipal(const sf::Event& event) {
    switch (event.key.code) {
        case sf::Keyboard::Up:
            opcionMenuSeleccionada = (opcionMenuSeleccionada - 1 + CANTIDAD_OPCIONES_MENU) % CANTIDAD_OPCIONES_MENU;
            return MenuResult::NONE;

        case sf::Keyboard::Down:
            opcionMenuSeleccionada = (opcionMenuSeleccionada + 1) % CANTIDAD_OPCIONES_MENU;
            return MenuResult::NONE;

        case sf::Keyboard::Return:
            switch (opcionMenuSeleccionada) {
                case 0: return MenuResult::PLAY;
                case 1: return MenuResult::HIGH_SCORES;
                case 2: return MenuResult::QUIT;
            }
            break;

        default:
            break;
    }
    return MenuResult::NONE;
}

// ============================================================================
// manejarEntradaSeleccionPersonaje - Navegar entre personajes y confirmar
// ============================================================================
MenuResult Menu::manejarEntradaSeleccionPersonaje(const sf::Event& event) {
    int charCount = static_cast<int>(datosPersonajes.size());
    if (charCount == 0) return MenuResult::BACK;

    switch (event.key.code) {
        case sf::Keyboard::Left:
            personajeSeleccionado = (personajeSeleccionado - 1 + charCount) % charCount;
            return MenuResult::NONE;

        case sf::Keyboard::Right:
            personajeSeleccionado = (personajeSeleccionado + 1) % charCount;
            return MenuResult::NONE;

        case sf::Keyboard::Return:
            return MenuResult::CHARACTER_SELECTED;

        case sf::Keyboard::Escape:
            return MenuResult::BACK;

        default:
            break;
    }
    return MenuResult::NONE;
}

// Game Over: navegar entre Reintentar y Menú Principal
MenuResult Menu::manejarEntradaGameOver(const sf::Event& event) {
    switch (event.key.code) {
        case sf::Keyboard::Up:
        case sf::Keyboard::Down:
            // Alternar entre las dos opciones (0 y 1)
            opcionGameOverSeleccionada = 1 - opcionGameOverSeleccionada;
            return MenuResult::NONE;

        case sf::Keyboard::Return:
            if (opcionGameOverSeleccionada == 0) {
                return MenuResult::RETRY;  // Reintentar
            } else {
                return MenuResult::BACK;   // Volver al menú
            }

        default:
            break;
    }
    return MenuResult::NONE;
}

// Tabla de récords: solo ESC para volver
MenuResult Menu::manejarEntradaHighScores(const sf::Event& event) {
    if (event.key.code == sf::Keyboard::Escape) {
        return MenuResult::BACK;
    }
    return MenuResult::NONE;
}

// ============================================================================
// dibujar - Dibujar el menú según el estado actual
// ============================================================================
void Menu::dibujar(sf::RenderWindow& window, MenuState estado) {
    static sf::Clock pulseClock;
    timerPulsoTitulo = pulseClock.getElapsedTime().asSeconds();

    switch (estado) {
        case MenuState::MENU_PRINCIPAL:
            dibujarMenuPrincipal(window);
            break;
        case MenuState::SELECCION_PERSONAJE:
            dibujarSeleccionPersonaje(window);
            break;
        case MenuState::GAME_OVER:
            dibujarGameOver(window);
            break;
        case MenuState::HIGH_SCORES:
            dibujarHighScores(window);
            break;
    }
}

// ============================================================================
// Métodos de renderizado por estado
// ============================================================================

// ============================================================================
// dibujarMenuPrincipal - Renderizar el menú principal con título animado
// ============================================================================
void Menu::dibujarMenuPrincipal(sf::RenderWindow& window) {
    sf::RectangleShape background(sf::Vector2f(1280.0f, 720.0f));
    background.setFillColor(sf::Color(10, 10, 15, 220));
    window.draw(background);

    float pulse = 1.0f + 0.03f * std::sin(timerPulsoTitulo * 2.0f);
    titulo.setScale(pulse, pulse);
    centrarTextoHorizontalmente(titulo, 120.0f);

    window.draw(titulo);
    window.draw(subtitulo);

    for (int i = 0; i < CANTIDAD_OPCIONES_MENU; i++) {
        if (i == opcionMenuSeleccionada) {
            opcionesMenu[i].setFillColor(sf::Color(255, 255, 100));
            opcionesMenu[i].setCharacterSize(32);
            std::string labels[CANTIDAD_OPCIONES_MENU] = {"JUGAR", "RECORDS", "SALIR"};
            opcionesMenu[i].setString("> " + labels[i] + " <");
        } else {
            opcionesMenu[i].setFillColor(sf::Color(200, 200, 200));
            opcionesMenu[i].setCharacterSize(28);
            std::string labels[CANTIDAD_OPCIONES_MENU] = {"JUGAR", "RECORDS", "SALIR"};
            opcionesMenu[i].setString(labels[i]);
        }
        centrarTextoHorizontalmente(opcionesMenu[i], 350.0f + i * 60.0f);
        window.draw(opcionesMenu[i]);
    }

    sf::Text footerText;
    footerText.setFont(*font);
    footerText.setCharacterSize(14);
    footerText.setFillColor(sf::Color(100, 100, 100));
    footerText.setString("UTN - Programacion II - Usa las flechas y ENTER para navegar");
    centrarTextoHorizontalmente(footerText, 680.0f);
    window.draw(footerText);
}

// ============================================================================
// dibujarSeleccionPersonaje - Renderizar la pantalla de selección de personaje
// ============================================================================
void Menu::dibujarSeleccionPersonaje(sf::RenderWindow& window) {
    sf::RectangleShape background(sf::Vector2f(1280.0f, 720.0f));
    background.setFillColor(sf::Color(10, 10, 15, 220));
    window.draw(background);

    window.draw(tituloSeleccion);

    for (size_t i = 0; i < nombresPersonajes.size(); i++) {
        bool isSelected = (static_cast<int>(i) == personajeSeleccionado);

        float panelX = nombresPersonajes[i].getPosition().x - 15.0f;
        float panelY = nombresPersonajes[i].getPosition().y - 10.0f;
        sf::RectangleShape charPanel(sf::Vector2f(this->cardWidth, 230.0f));
        charPanel.setPosition(panelX, panelY);

        if (isSelected) {
            charPanel.setFillColor(sf::Color(40, 40, 60, 200));
            charPanel.setOutlineColor(sf::Color(255, 255, 100));
            charPanel.setOutlineThickness(2.0f);
            nombresPersonajes[i].setFillColor(sf::Color(255, 255, 100));
        } else {
            charPanel.setFillColor(sf::Color(20, 20, 30, 150));
            charPanel.setOutlineColor(sf::Color(80, 80, 80));
            charPanel.setOutlineThickness(1.0f);
            nombresPersonajes[i].setFillColor(sf::Color(150, 150, 150));
        }

        window.draw(charPanel);
        if (i < spritesPersonajes.size()) {
            sf::Sprite sp = spritesPersonajes[i];
            sp.setPosition(nombresPersonajes[i].getPosition().x + this->cardWidth/2.0f - sp.getGlobalBounds().width/2.0f,
                           nombresPersonajes[i].getPosition().y + 10.0f);
            sp.setColor(isSelected ? sf::Color(255,255,255) : sf::Color(100,100,100));
            window.draw(sp);
        }
        window.draw(nombresPersonajes[i]);

        if (i < statsPersonajes.size()) {
            if (isSelected) {
                statsPersonajes[i].setFillColor(sf::Color(220, 220, 220));
            } else {
                statsPersonajes[i].setFillColor(sf::Color(140, 140, 140));
            }
            window.draw(statsPersonajes[i]);
        }
    }

    window.draw(instruccionesSeleccion);
}

// ============================================================================
// dibujarGameOver - Renderizar la pantalla de Game Over
// ============================================================================
void Menu::dibujarGameOver(sf::RenderWindow& window) {
    sf::RectangleShape background(sf::Vector2f(1280.0f, 720.0f));
    background.setFillColor(sf::Color(20, 5, 5, 230));
    window.draw(background);

    float pulse = 1.0f + 0.02f * std::sin(timerPulsoTitulo * 3.0f);
    tituloGameOver.setScale(pulse, pulse);
    centrarTextoHorizontalmente(tituloGameOver, 100.0f);
    window.draw(tituloGameOver);

    window.draw(textoPuntuacionFinal);
    window.draw(textoOleadaFinal);
    window.draw(textoTiempoFinal);

    sf::RectangleShape separator(sf::Vector2f(300.0f, 2.0f));
    separator.setFillColor(sf::Color(220, 50, 50, 150));
    separator.setPosition(490.0f, 410.0f);
    window.draw(separator);

    if (opcionGameOverSeleccionada == 0) {
        opcionReintentar.setFillColor(sf::Color(255, 255, 100));
        opcionReintentar.setString("> REINTENTAR <");
        opcionMenu.setFillColor(sf::Color(200, 200, 200));
        opcionMenu.setString("MENU PRINCIPAL");
    } else {
        opcionReintentar.setFillColor(sf::Color(200, 200, 200));
        opcionReintentar.setString("REINTENTAR");
        opcionMenu.setFillColor(sf::Color(255, 255, 100));
        opcionMenu.setString("> MENU PRINCIPAL <");
    }
    centrarTextoHorizontalmente(opcionReintentar, 450.0f);
    centrarTextoHorizontalmente(opcionMenu, 510.0f);

    window.draw(opcionReintentar);
    window.draw(opcionMenu);
}

// ============================================================================
// dibujarHighScores - Renderizar la tabla de récords (mejores puntuaciones)
// ============================================================================
void Menu::dibujarHighScores(sf::RenderWindow& window) {
    sf::RectangleShape background(sf::Vector2f(1280.0f, 720.0f));
    background.setFillColor(sf::Color(10, 10, 15, 230));
    window.draw(background);

    window.draw(tituloScores);

    sf::Text header;
    header.setFont(*font);
    header.setCharacterSize(18);
    header.setFillColor(sf::Color(255, 215, 0));
    header.setStyle(sf::Text::Bold);
    header.setString("  #     JUGADOR           PUNTOS    TIEMPO");
    header.setPosition(300.0f, 130.0f);
    window.draw(header);

    sf::RectangleShape headerLine(sf::Vector2f(600.0f, 1.0f));
    headerLine.setFillColor(sf::Color(255, 215, 0, 100));
    headerLine.setPosition(300.0f, 160.0f);
    window.draw(headerLine);

    for (size_t i = 0; i < entradasScores.size(); i++) {
        if (i % 2 == 0) {
            entradasScores[i].setFillColor(sf::Color(200, 200, 200));
        } else {
            entradasScores[i].setFillColor(sf::Color(170, 170, 170));
        }
        window.draw(entradasScores[i]);
    }

    if (entradasScores.empty()) {
        sf::Text noScores;
        noScores.setFont(*font);
        noScores.setCharacterSize(20);
        noScores.setFillColor(sf::Color(150, 150, 150));
        noScores.setString("No hay puntuaciones registradas aun");
        centrarTextoHorizontalmente(noScores, 350.0f);
        window.draw(noScores);
    }

    window.draw(textoVolver);
}

// ============================================================================
// establecerDatosGameOver - Almacenar datos del fin de partida
// ============================================================================
void Menu::establecerDatosGameOver(int score, int wave, float time) {
    std::ostringstream scoreSS;
    scoreSS << "PUNTUACION: " << score;
    textoPuntuacionFinal.setString(scoreSS.str());
    centrarTextoHorizontalmente(textoPuntuacionFinal, 250.0f);

    std::ostringstream waveSS;
    waveSS << "OLEADA ALCANZADA: " << wave;
    textoOleadaFinal.setString(waveSS.str());
    centrarTextoHorizontalmente(textoOleadaFinal, 300.0f);

    std::ostringstream timeSS;
    timeSS << "TIEMPO: " << formatearTiempo(time);
    textoTiempoFinal.setString(timeSS.str());
    centrarTextoHorizontalmente(textoTiempoFinal, 340.0f);

    opcionGameOverSeleccionada = 0;
}

// ============================================================================
// establecerHighScores - Preparar la tabla de puntuaciones para mostrar
// ============================================================================
void Menu::establecerHighScores(const std::vector<Estadistica>& stats) {
    entradasScores.clear();

    std::vector<Estadistica> sorted = stats;
    std::sort(sorted.begin(), sorted.end(),
        [](const Estadistica& a, const Estadistica& b) {
            return a.getPuntuacion() > b.getPuntuacion();
        }
    );

    int maxEntries = std::min(static_cast<int>(sorted.size()), 10);

    for (int i = 0; i < maxEntries; i++) {
        std::ostringstream ss;
        ss << std::setw(3) << (i + 1) << ".   "
           << std::setw(18) << std::left << sorted[i].getNombreJugador()
           << std::setw(8) << std::right << sorted[i].getPuntuacion()
           << "    " << formatearTiempo(static_cast<float>(sorted[i].getTiempoJugado()));

        sf::Text entry;
        entry.setFont(*font);
        entry.setCharacterSize(18);
        entry.setString(ss.str());
        entry.setPosition(310.0f, 175.0f + i * 40.0f);

        if (i == 0) {
            entry.setFillColor(sf::Color(255, 215, 0));
        } else if (i == 1) {
            entry.setFillColor(sf::Color(192, 192, 192));
        } else if (i == 2) {
            entry.setFillColor(sf::Color(205, 127, 50));
        }

        entradasScores.push_back(entry);
    }
}

// ============================================================================
// getPersonajeSeleccionado - Retornar índice del personaje seleccionado
// ============================================================================
int Menu::getPersonajeSeleccionado() const {
    return personajeSeleccionado;
}

// ============================================================================
// setMenuState / getMenuState - Gestión del estado del menú
// ============================================================================
void Menu::setMenuState(MenuState estado) {
    estadoActual = estado;
}

MenuState Menu::getMenuState() const {
    return estadoActual;
}

// ============================================================================
// centrarTextoHorizontalmente - Centrar un texto en la pantalla
// ============================================================================
void Menu::centrarTextoHorizontalmente(sf::Text& text, float yPosition) {
    // getLocalBounds() retorna el rectángulo que contiene el texto
    sf::FloatRect bounds = text.getLocalBounds();

    // Calcular posición X para centrar:
    // (ancho de ventana - ancho del texto) / 2
    // Restamos bounds.left para compensar el offset interno del texto
    float xPos = (1280.0f - bounds.width) / 2.0f - bounds.left;

    text.setPosition(xPos, yPosition);
}

// ============================================================================
// formatearTiempo - Convertir segundos a formato MM:SS
// ============================================================================
std::string Menu::formatearTiempo(float seconds) const {
    int totalSeconds = static_cast<int>(seconds);
    int minutes = totalSeconds / 60;
    int secs = totalSeconds % 60;

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes
       << ":"
       << std::setfill('0') << std::setw(2) << secs;

    return ss.str();
}

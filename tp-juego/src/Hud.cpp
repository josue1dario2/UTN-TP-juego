#include "../include/Hud.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Hud::Hud() {
    juegoTerminado = false;
}

bool Hud::inicializar() {
    if (!fuente.loadFromFile("assets/font.ttf")) {
        std::cerr << "Error: No se pudo cargar assets/font.ttf" << std::endl;
        return false;
    }

    // Configurar Textos
    configurarTexto(textoVida, 16, sf::Color::White);
    configurarTexto(textoArmadura, 16, sf::Color::White);
    configurarTexto(textoArma, 18, sf::Color::Yellow);
    configurarTexto(textoMunicion, 22, sf::Color::White);
    configurarTexto(textoOleada, 26, sf::Color(255, 215, 0)); // Color Dorado/Amarillo para la oleada
    configurarTexto(textoEstadoOleada, 14, sf::Color(200, 200, 200));
    configurarTexto(textoJuegoTerminado, 48, sf::Color::Red);
    textoJuegoTerminado.setString("JUEGO TERMINADO");

    // Configurar Barras
    float anchoBarra = 180.f;
    float altoBarra = 14.f;

    fondoBarraVida.setSize(sf::Vector2f(anchoBarra, altoBarra));
    fondoBarraVida.setFillColor(sf::Color(50, 0, 0, 150));
    fondoBarraVida.setOutlineColor(sf::Color::White);
    fondoBarraVida.setOutlineThickness(1.f);

    barraVida.setSize(sf::Vector2f(anchoBarra, altoBarra));
    barraVida.setFillColor(sf::Color(220, 50, 50)); // Rojo intenso para vida

    fondoBarraArmadura.setSize(sf::Vector2f(anchoBarra, altoBarra));
    fondoBarraArmadura.setFillColor(sf::Color(0, 0, 50, 150));
    fondoBarraArmadura.setOutlineColor(sf::Color::White);
    fondoBarraArmadura.setOutlineThickness(1.f);

    barraArmadura.setSize(sf::Vector2f(anchoBarra, altoBarra));
    barraArmadura.setFillColor(sf::Color(50, 120, 240)); // Azul brillante para escudo

    // Paneles de fondo estéticos (con transparencias)
    panelJugador.setFillColor(sf::Color(0, 0, 0, 180));
    panelJugador.setOutlineColor(sf::Color(100, 100, 100));
    panelJugador.setOutlineThickness(1.5f);

    panelArma.setFillColor(sf::Color(0, 0, 0, 180));
    panelArma.setOutlineColor(sf::Color(100, 100, 100));
    panelArma.setOutlineThickness(1.5f);

    panelOleada.setFillColor(sf::Color(0, 0, 0, 180));
    panelOleada.setOutlineColor(sf::Color(100, 100, 100)); // Color gris neutro para el contorno
    panelOleada.setOutlineThickness(1.5f);

    return true;
}

void Hud::configurarTexto(sf::Text& texto, unsigned int tamanio, sf::Color color) {
    texto.setFont(fuente);
    texto.setCharacterSize(tamanio);
    texto.setFillColor(color);
    texto.setOutlineColor(sf::Color::Black);
    texto.setOutlineThickness(1.5f);
}

void Hud::actualizar(const Personaje& jugador, const ZombieManager& zombieManager) {
    // 1. Vida del Jugador
    float vidaAct = jugador.getVidaActual();
    juegoTerminado = (vidaAct <= 0.f);
    
    float vidaMax = jugador.getVidaMax();
    if (vidaMax > 0.f) {
        float pctVida = vidaAct / vidaMax;
        if (pctVida < 0.f) pctVida = 0.f;
        barraVida.setSize(sf::Vector2f(180.f * pctVida, 14.f));
    }
    textoVida.setString("VIDA: " + std::to_string((int)vidaAct) + " / " + std::to_string((int)vidaMax));

    // 2. Armadura/Escudo del Jugador
    float armAct = jugador.getArmaduraActual();
    float armMax = jugador.getArmaduraMax();
    if (armMax > 0.f) {
        float pctArm = armAct / armMax;
        if (pctArm < 0.f) pctArm = 0.f;
        barraArmadura.setSize(sf::Vector2f(180.f * pctArm, 14.f));
    }
    textoArmadura.setString("ESCUDO: " + std::to_string((int)armAct) + " / " + std::to_string((int)armMax));

    // 3. Arma y Munición
    Arma& arma = const_cast<Personaje&>(jugador).getArma();
    textoArma.setString(arma.getNombre());
    if (arma.getEnRecarga()) {
        textoMunicion.setString("RECARGANDO...");
        textoMunicion.setFillColor(sf::Color(255, 150, 0)); // Color naranja para recarga
    } else {
        textoMunicion.setString(std::to_string(arma.getMunicionEnCargador()) + " / " + std::to_string(arma.getMunicionActual()));
        textoMunicion.setFillColor(sf::Color::White);
    }

    // 4. Lógica de Oleadas y Cooldown de Tregua
    textoOleada.setString("OLEADA " + std::to_string(zombieManager.getOleadaActual()));

    if (zombieManager.getEnPeriodoDescanso()) {
        float tiempoRestante = zombieManager.getTiempoTotalDescanso() - zombieManager.getCronometroDescanso();
        if (tiempoRestante < 0.f) tiempoRestante = 0.f;
        int segundosRestantes = static_cast<int>(std::ceil(tiempoRestante));
        
        textoEstadoOleada.setString("PROXIMA OLEADA EN: " + std::to_string(segundosRestantes));
        textoEstadoOleada.setFillColor(sf::Color::Green); // Color verde para tranquilidad
    } else {
        int restantes = zombieManager.getZombiesActivos() + zombieManager.getZombiesRestantesPorCrear();
        textoEstadoOleada.setString("ZOMBIES EN OLEADA: " + std::to_string(restantes));
        textoEstadoOleada.setFillColor(sf::Color(200, 200, 200));
    }
}

void Hud::dibujar(sf::RenderWindow& ventana) {
    // Guardar vista de juego actual
    sf::View vistaActual = ventana.getView();

    // Establecer la vista estática de pantalla
    ventana.setView(ventana.getDefaultView());

    sf::Vector2f resolucion = ventana.getView().getSize();
    float width = resolucion.x;
    float height = resolucion.y;

    if (juegoTerminado) {
        // Mostrar cartel centrado de juego terminado
        textoJuegoTerminado.setPosition(width / 2.f - (textoJuegoTerminado.getLocalBounds().width / 2.f), height / 2.f - (textoJuegoTerminado.getLocalBounds().height / 2.f));
        ventana.draw(textoJuegoTerminado);
    } else {
        // --- POSICIONAR ELEMENTOS ---

        // Panel Jugador (Abajo a la Izquierda)
        panelJugador.setSize(sf::Vector2f(230.f, 95.f));
        panelJugador.setPosition(20.f, height - 115.f);

        textoVida.setPosition(35.f, height - 105.f);
        fondoBarraVida.setPosition(35.f, height - 85.f);
        barraVida.setPosition(35.f, height - 85.f);

        textoArmadura.setPosition(35.f, height - 65.f);
        fondoBarraArmadura.setPosition(35.f, height - 45.f);
        barraArmadura.setPosition(35.f, height - 45.f);

        // Panel Arma y Munición (Abajo a la Derecha)
        panelArma.setSize(sf::Vector2f(220.f, 85.f));
        panelArma.setPosition(width - 240.f, height - 105.f);

        float posCentroPanelArma = width - 130.f;
        textoArma.setPosition(posCentroPanelArma - (textoArma.getLocalBounds().width / 2.f), height - 95.f);
        textoMunicion.setPosition(posCentroPanelArma - (textoMunicion.getLocalBounds().width / 2.f), height - 70.f);

        // Panel Oleada (Arriba en el Centro - Desplazado 20px más abajo)
        panelOleada.setSize(sf::Vector2f(320.f, 75.f));
        panelOleada.setPosition(width / 2.f - 160.f, 35.f);

        textoOleada.setPosition(width / 2.f - (textoOleada.getLocalBounds().width / 2.f), 40.f);
        textoEstadoOleada.setPosition(width / 2.f - (textoEstadoOleada.getLocalBounds().width / 2.f), 70.f);

        // --- DIBUJAR ---
        ventana.draw(panelJugador);
        ventana.draw(panelArma);
        ventana.draw(panelOleada);

        ventana.draw(fondoBarraVida);
        ventana.draw(barraVida);
        ventana.draw(fondoBarraArmadura);
        ventana.draw(barraArmadura);

        ventana.draw(textoVida);
        ventana.draw(textoArmadura);
        ventana.draw(textoArma);
        ventana.draw(textoMunicion);
        ventana.draw(textoOleada);
        ventana.draw(textoEstadoOleada);
    }

    // Restaurar vista original
    ventana.setView(vistaActual);
}

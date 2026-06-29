#include "../include/Hud.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Hud::Hud() {
    juegoTerminado = false;
}

bool Hud::inicializar(int idJug) {

    texturaIcon.loadFromFile("assets/personajes/icon_" + std::to_string(idJug) + ".png");
    spriteIcon.setTexture(texturaIcon);
    //spriteIcon.setPosition(sf::Vector2f(200,1800));
    spriteIcon.setScale(0.1f, 0.1f);

    texturaDinero.loadFromFile("assets/varios/dinero.png");
    spriteDinero.setTexture(texturaDinero);
    spriteDinero.setScale(0.5f,0.5f);
    

    if (!fuente.loadFromFile("assets/minecraft.ttf")) {
        std::cerr << "Error: No se pudo cargar assets/minecraft.ttf" << std::endl;
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
    configurarTexto(textoDineroJugador, 16, sf::Color::White);

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

    configurarTexto(textoHabilidad, 16, sf::Color::White);

    fondoBarraHabilidad.setSize(sf::Vector2f(anchoBarra, altoBarra));
    fondoBarraHabilidad.setFillColor(sf::Color(40, 30, 0, 150));
    fondoBarraHabilidad.setOutlineColor(sf::Color::White);
    fondoBarraHabilidad.setOutlineThickness(1.f);

    barraHabilidad.setSize(sf::Vector2f(anchoBarra, altoBarra));
    barraHabilidad.setFillColor(sf::Color(240, 200, 30)); // Color dorado para habilidad

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
    
    // recursos
    textoDineroJugador.setString("$" + std::to_string(jugador.getDinero()));
    
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

    // 2b. Habilidad del Jugador
    float tiempoHabilidad = jugador.getTiempoHabilidad();
    float cooldownHabilidad = jugador.getCooldownHabilidad();
    bool disp = jugador.getHabilidadDisponible();
    bool activa = jugador.habilidadActiva();

    float pctHab = 0.f;
    if (disp) {
        pctHab = 1.f;
        textoHabilidad.setString("HABILIDAD: LISTA [E]");
        textoHabilidad.setFillColor(sf::Color(255, 215, 0)); // Dorado
        barraHabilidad.setFillColor(sf::Color(255, 215, 0));
    } else if (activa) {
        pctHab = 1.f;
        textoHabilidad.setString("HABILIDAD: ACTIVA");
        textoHabilidad.setFillColor(sf::Color(50, 220, 50)); // Verde
        barraHabilidad.setFillColor(sf::Color(50, 220, 50));
    } else {
        pctHab = tiempoHabilidad / cooldownHabilidad;
        if (pctHab > 1.f) pctHab = 1.f;
        if (pctHab < 0.f) pctHab = 0.f;
        
        float tiempoRestante = cooldownHabilidad - tiempoHabilidad;
        if (tiempoRestante < 0.f) tiempoRestante = 0.f;
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << tiempoRestante;
        textoHabilidad.setString("HABILIDAD: " + ss.str() + "s");
        textoHabilidad.setFillColor(sf::Color(200, 200, 200)); // Gris
        barraHabilidad.setFillColor(sf::Color(240, 150, 30)); // Naranja de recarga
    }
    barraHabilidad.setSize(sf::Vector2f(180.f * pctHab, 14.f));

    // 3. Arma y Munición
    Arma& arma = const_cast<Personaje&>(jugador).getArma();
    spriteArmaUI = arma.getSprite();
    spriteArmaUI.setRotation(0.f);
    
    // Ajustar escala según el arma para equilibrar el tamaño visual en la UI
    int idArma = arma.getIdArma();
    if (idArma == 0) {       // Cuchillo
        spriteArmaUI.setScale(5.0f, 5.0f);
    } else if (idArma == 1) { // Pistola
        spriteArmaUI.setScale(4.5f, 4.5f);
    } else {                  // Escopeta, Rifle, Mosin
        spriteArmaUI.setScale(2.5f, 2.5f);
    }
    
    sf::FloatRect bounds = spriteArmaUI.getLocalBounds();
    spriteArmaUI.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

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

        spriteIcon.setPosition(30.f, height - 170.f);

        // Panel Jugador (Abajo a la Izquierda)
        panelJugador.setSize(sf::Vector2f(350.f, 150.f));
        panelJugador.setPosition(20.f, height - 180.f);

        textoVida.setPosition(160.f, height - 165.f);
        fondoBarraVida.setPosition(160.f, height - 145.f);
        barraVida.setPosition(160.f, height - 145.f);

        textoArmadura.setPosition(160.f, height - 125.f);
        fondoBarraArmadura.setPosition(160.f, height - 105.f);
        barraArmadura.setPosition(160.f, height - 105.f);

        textoHabilidad.setPosition(160.f, height - 85.f);
        fondoBarraHabilidad.setPosition(160.f, height - 65.f);
        barraHabilidad.setPosition(160.f, height - 65.f);

        spriteDinero.setPosition(160.f, height - 45.f);
        textoDineroJugador.setPosition(215.f, height - 47.f);

        // Panel Arma y Munición (Abajo a la Derecha)
        panelArma.setSize(sf::Vector2f(220.f, 85.f));
        panelArma.setPosition(width - 240.f, height - 105.f);

        float posCentroPanelArma = width - 130.f;
        spriteArmaUI.setPosition(posCentroPanelArma, height - 90.f);
        textoMunicion.setPosition(posCentroPanelArma - (textoMunicion.getLocalBounds().width / 2.f), height - 55.f);

        // Panel Oleada (Arriba en el Centro - Desplazado 20px más abajo)
        panelOleada.setSize(sf::Vector2f(320.f, 75.f));
        panelOleada.setPosition(width / 2.f - 160.f, 35.f);

        textoOleada.setPosition(width / 2.f - (textoOleada.getLocalBounds().width / 2.f), 40.f);
        textoEstadoOleada.setPosition(width / 2.f - (textoEstadoOleada.getLocalBounds().width / 2.f), 70.f);

        // --- DIBUJAR ---
        // ventana.draw(panelJugador); // Mantener paneles invisibles como se solicitó
        // ventana.draw(panelArma);
        // ventana.draw(panelOleada);

        ventana.draw(fondoBarraVida);
        ventana.draw(barraVida);
        ventana.draw(fondoBarraArmadura);
        ventana.draw(barraArmadura);
        ventana.draw(fondoBarraHabilidad);
        ventana.draw(barraHabilidad);

        ventana.draw(textoVida);
        ventana.draw(textoArmadura);
        ventana.draw(textoHabilidad);
        ventana.draw(spriteArmaUI);
        ventana.draw(textoMunicion);
        ventana.draw(textoOleada);
        ventana.draw(textoEstadoOleada);
        ventana.draw(textoDineroJugador);
        ventana.draw(spriteIcon);
        ventana.draw(spriteDinero);
    }
    
    // Restaurar vista original
    ventana.setView(vistaActual);
}

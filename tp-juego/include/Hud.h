#pragma once
#include <SFML/Graphics.hpp>
#include "Personaje.h"
#include "ZombieManager.h"
#include <cmath>

class Hud {
private:
    sf::Font fuente;

    // Elementos de Texto para la UI
    sf::Text textoVida;
    sf::Text textoArmadura;
    sf::Text textoArma;
    sf::Text textoMunicion;
    sf::Text textoOleada;
    sf::Text textoEstadoOleada; // Muestra "PRÓXIMA OLEADA EN X" o "ZOMBIES RESTANTES"
    sf::Text textoJuegoTerminado; // Muestra "JUEGO TERMINADO" cuando la vida es 0
    sf::Text textoDineroJugador;

    // Barras gráficas para Vida
    sf::RectangleShape fondoBarraVida;
    sf::RectangleShape barraVida;

    // Barras gráficas para Armadura
    sf::RectangleShape fondoBarraArmadura;
    sf::RectangleShape barraArmadura;

    // Barras gráficas para Habilidad
    sf::Text textoHabilidad;
    sf::RectangleShape fondoBarraHabilidad;
    sf::RectangleShape barraHabilidad;

    // Contenedores/fondos estéticos para agrupar la información (estilo panel)
    sf::RectangleShape panelJugador;
    sf::RectangleShape panelOleada;
    sf::RectangleShape panelArma;

    sf::Texture texturaIcon;
    sf::Sprite spriteIcon;

    sf::Texture texturaDinero;
    sf::Sprite spriteDinero;

    // Función auxiliar para configurar estilos de texto comunes
    void configurarTexto(sf::Text& texto, unsigned int tamanio, sf::Color color);
    
    sf::Sprite spriteArmaUI;
    bool juegoTerminado;

public:
    Hud();
    bool inicializar(int idJug);
    void actualizar(const Personaje& jugador, const ZombieManager& zombieManager);
    void dibujar(sf::RenderWindow& ventana);
};


#pragma once

#include <SFML/Graphics.hpp>

enum class OpcionMenu {
    NuevaPartida,
    ContinuarPartida,
    Salir
};

enum class TipoPersonaje {
    Superviviente = 0,
    Tanque = 1,
    Explorador = 2
};

enum class OpcionPausa {
    Seguir,
    Salir
};

class Menu {
private:
    sf::RenderWindow ventana;
    sf::View vista;
    sf::Font fuente;

    int opcionActual;
    int personajeActual;

    void dibujarBoton(sf::RectangleShape& boton, float x, float y, bool seleccionado);

public:
    Menu();

    OpcionMenu mostrarMenuPrincipal();
    OpcionPausa mostrarMenuPausa(sf::RenderWindow& ventana);
    TipoPersonaje mostrarSeleccionPersonaje();
};
#pragma once
#include <SFML/Graphics.hpp>
#include "Personaje.h" 

class Juego {
    private:
        sf::RenderWindow ventana;
        sf::View vista;

        Personaje jugador; 

        sf::Clock relojDelta;
        float deltaTime;

        void procesarEventos();
        void actualizar();
        void renderizar();

    public:
        Juego();
        void iniciar();       
};
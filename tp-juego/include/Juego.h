#pragma once
#include <SFML/Graphics.hpp>
#include "Personaje.h" 

class Juego {
    private:
        sf::RenderWindow ventana;
        Personaje jugador; 

        sf::Clock relojDelta;
        float deltaTime;

        void procesarEventos();
        void actualizar();
        void renderizar(sf::RectangleShape rect);

    public:
        Juego();
        void iniciar();       
};
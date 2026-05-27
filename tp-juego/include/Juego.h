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

        sf::Texture texturaMapa;
        sf::Sprite spriteMapa;
        sf::Image imagenMapaColision;

        void procesarEventos();
        void actualizar();
        void renderizar();

    public:
        Juego();
        void iniciar();       
};
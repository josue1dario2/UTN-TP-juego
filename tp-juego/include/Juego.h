#pragma once
#include <SFML/Graphics.hpp>

class Juego {
    private:
        sf::RenderWindow ventana;

        sf::Clock relojDelta;
        float deltaTime;

        void procesarEventos();
        void actualizar();
        void renderizar(sf::RectangleShape rect);

    public:
        Juego();
        void iniciar();       
};

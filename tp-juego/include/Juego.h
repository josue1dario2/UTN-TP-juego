#pragma once
#include <SFML/Graphics.hpp>

class Juego {
    private:
        sf::RenderWindow ventana;

        void procesarEventos();
        void actualizar();
        void renderizar();

    public:
        Juego();
        void iniciar();       
};

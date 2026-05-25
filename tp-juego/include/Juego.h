#pragma once
#include <SFML/Graphics.hpp>
#include "Personaje.h"
#include "ObjetoMapa.h"

class Juego {
    private:
        // VENTANA Y VISTA
        sf::RenderWindow ventana;
        sf::View vista;

        // RELOJ PARA CONTROLAR EL TIEMPO ENTRE FRAMES
        sf::Clock relojDelta;
        float deltaTime;

        // INCIALIZACION DE ELEMENTOS DEL JUEGO
        Personaje jugador; 
        ObjetoMapa casa;


        sf::Texture texturaMapa;
        sf::Sprite spriteMapa;

        void procesarEventos();
        void actualizar();
        void renderizar();

    public:
        Juego();
        void iniciar();       
};
#pragma once
#include <SFML/Graphics.hpp>
#include "Personaje.h"
#include "ObjetoMapa.h"
#include <vector>

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

        std::vector<ObjetoMapa> obstaculos; // Vector para almacenar múltiples elementos del mapa/paredes/obstaculos




        sf::Texture texturaMapa;
        sf::Sprite spriteMapa;

        void procesarEventos();
        void actualizar();
        void renderizar();

        void inicializarObstaculos(std::vector<ObjetoMapa>& obstaculos);

    public:
        Juego();
        void iniciar();       
};
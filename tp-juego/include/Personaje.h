#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Personaje {
private:
    int idPersonaje;
    std::string nombre;
    float vidaMax;
    float vidaActual;
    float armaduraMax;
    float armaduraActual;
    float velocidad;
    std::string habilidad;

    sf::RectangleShape cuerpo;

public:

    Personaje();

    void cargarAtributos(int id, std::string nom, float vida, float armadura, float vel, std::string hab);

  
    void controlar();   
                        // Movimiento con W, A, S, D
    void dibujar(sf::RenderWindow& ventana); // Renderizado en pantalla

    sf::Vector2f getPosition() const { return cuerpo.getPosition(); }
};
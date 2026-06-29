#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Boton {
private:
    sf::RectangleShape caja;
    sf::Text texto;
    sf::Color colorNormal;
    sf::Color colorHover;
    sf::Color colorClick;
    bool hover;

public:
    Boton() : hover(false) {}

    Boton(float x, float y, float ancho, float alto, sf::Font& fuente, std::string textoStr, 
          sf::Color normal, sf::Color colHover, sf::Color click) {
        caja.setPosition(x, y);
        caja.setSize({ancho, alto});
        colorNormal = normal;
        colorHover = colHover;
        colorClick = click;
        caja.setFillColor(colorNormal);
        caja.setOutlineColor(sf::Color(140, 50, 30)); // Borde óxido/naranja oscuro
        caja.setOutlineThickness(2.0f);

        texto.setFont(fuente);
        texto.setString(textoStr);
        texto.setCharacterSize(16);
        texto.setFillColor(sf::Color(240, 235, 225)); // Texto claro/crema
        
        // Centrar texto en el botón
        sf::FloatRect bounds = texto.getLocalBounds();
        texto.setOrigin(bounds.left + bounds.width/2.0f, bounds.top + bounds.height/2.0f);
        texto.setPosition(x + ancho/2.0f, y + alto/2.0f);
        
        this->hover = false;
    }

    void actualizar(sf::Vector2f posMouse, bool forzado = false) {
        if (forzado || caja.getGlobalBounds().contains(posMouse)) {
            hover = true;
            caja.setFillColor(colorHover);
            caja.setOutlineColor(sf::Color(230, 140, 40)); // Borde naranja brillante en hover
            texto.setFillColor(sf::Color(255, 215, 0)); // Texto dorado en hover
        } else {
            hover = false;
            caja.setFillColor(colorNormal);
            caja.setOutlineColor(sf::Color(140, 50, 30)); // Borde óxido normal
            texto.setFillColor(sf::Color(240, 235, 225)); // Texto normal
        }
    }

    void setTextoPosicion(float x, float y) {
        sf::FloatRect bounds = texto.getLocalBounds();
        texto.setOrigin(bounds.left + bounds.width/2.0f, bounds.top + bounds.height/2.0f);
        texto.setPosition(x, y);
    }

    void dibujar(sf::RenderWindow& ventana) {
        ventana.draw(caja);
        ventana.draw(texto);
    }

    bool fueClickeado(sf::Vector2f posMouse, sf::Mouse::Button boton) {
        return hover && boton == sf::Mouse::Left;
    }

    sf::FloatRect getGlobalBounds() const {
        return caja.getGlobalBounds();
    }
};

#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class ObjetoGrafico
{
protected:

    sf::Texture textura;
    sf::Sprite sprite;

public:

    ObjetoGrafico();

    //------------CONFIGURACION DE SPRITE------------
    bool cargarTextura(const std::string& ruta);
    void centrarOrigen();
    void setearTamanioSprite(int ancho, int alto);

    //------------POSICIONAMIENTO------------
    void setPosicion(float x, float y);
    void mover(float offsetX, float offsetY);

    //------------ACTUALIZACION Y DIBUJO------------
    virtual void actualizar(float deltaTime);
    virtual void dibujar(sf::RenderWindow& ventana);

    //------------GETTERS------------
    sf::Vector2f getPosicion() const;

};
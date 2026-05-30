#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class ObjetoGrafico
{
protected:

    sf::Texture textura;
    sf::Sprite sprite;

    float angulo;

    //------------HITBOX------------
    sf::FloatRect hitbox;

    sf::RectangleShape hitboxDebug;

    bool mostrarHitbox;

public:

    ObjetoGrafico();

    //------------CONFIGURACION DE SPRITE------------
    bool cargarTextura(const std::string& ruta);
    void centrarOrigen();
    void setearTamanioSprite(int ancho, int alto); //ajusta cuantos pixeles de la textura se muestran en el sprite
    void escalarSprite(float factorX, float factorY); //multiplica el tamaño del sprite por los factores dados

    //------------POSICIONAMIENTO------------
    void setPosicionCentrado(float x, float y); //tiene en cuenta el origen centrado del sprite para posicionar
    void setPosicion(float x, float y);
    void mover(float offsetX, float offsetY);

    //------------ACTUALIZACION Y DIBUJO------------
    virtual void actualizar(float deltaTime);
    virtual void dibujar(sf::RenderWindow& ventana);

    //------------GETTERS------------
    sf::Vector2f getPosicion() const;
    float getAngulo() const;
    void setAngulo(float nuevoAngulo);

    //------------HITBOX------------
    void setHitbox(float ancho, float alto);
    sf::FloatRect getHitbox() const;

};
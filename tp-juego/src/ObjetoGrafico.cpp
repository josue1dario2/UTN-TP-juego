#include "../include/ObjetoGrafico.h"

ObjetoGrafico::ObjetoGrafico()
{

}

bool ObjetoGrafico::cargarTextura(const std::string& ruta)
{
    if (!textura.loadFromFile(ruta))
    {
        return false;
    }

    sprite.setTexture(textura);

    return true;
}

void ObjetoGrafico::setPosicion(float x, float y)
{
    sprite.setPosition(x, y);
}

void ObjetoGrafico::mover(float offsetX, float offsetY)
{
    sprite.move(offsetX, offsetY);
}

void ObjetoGrafico::centrarOrigen()
{
    sf::FloatRect bounds = sprite.getLocalBounds();

    sprite.setOrigin(
        bounds.width / 2.f,
        bounds.height / 2.f
    );
}

void ObjetoGrafico::setearTamanioSprite(int ancho, int alto) {
    sprite.setTextureRect(sf::IntRect(0, 0, ancho, alto));
}

void ObjetoGrafico::actualizar(float deltaTime)
{

}

void ObjetoGrafico::dibujar(sf::RenderWindow& ventana)
{
    ventana.draw(sprite);
}

sf::Vector2f ObjetoGrafico::getPosicion() const
{
    return sprite.getPosition();
}
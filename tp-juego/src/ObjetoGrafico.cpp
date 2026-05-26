#include "../include/ObjetoGrafico.h"

ObjetoGrafico::ObjetoGrafico()
{
    //------------HITBOX------------
    mostrarHitbox = true;
    hitboxDebug.setFillColor(sf::Color::Transparent);
    hitboxDebug.setOutlineColor(sf::Color::Red);
    hitboxDebug.setOutlineThickness(2.f);

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

void ObjetoGrafico::setPosicionCentrado(float x, float y)
{
    sprite.setPosition(x, y);

    hitbox.left = x - hitbox.width / 2.f;
    hitbox.top = y - hitbox.height / 2.f;

    hitboxDebug.setPosition(hitbox.left, hitbox.top);
}

void ObjetoGrafico::setPosicion(float x, float y)
{
    sprite.setPosition(x, y);

    hitbox.left = x;
    hitbox.top = y;

    hitboxDebug.setPosition(hitbox.left, hitbox.top);
}

void ObjetoGrafico::mover(float offsetX, float offsetY)
{
    sprite.move(offsetX, offsetY);

    hitbox.left += offsetX;
    hitbox.top += offsetY;

    hitboxDebug.move(offsetX, offsetY);
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

void ObjetoGrafico::escalarSprite(float factorX, float factorY){
    sprite.setScale(factorX, factorY);
}

void ObjetoGrafico::actualizar(float deltaTime)
{

}

void ObjetoGrafico::dibujar(sf::RenderWindow& ventana)
{
    ventana.draw(sprite);

    if (mostrarHitbox)
    {
        ventana.draw(hitboxDebug);
    }
}

sf::Vector2f ObjetoGrafico::getPosicion() const
{
    return sprite.getPosition();
}

void ObjetoGrafico::setHitbox(float ancho, float alto)
{
    hitbox.width = ancho;
    hitbox.height = alto;

    hitboxDebug.setSize(sf::Vector2f(ancho, alto));
}

sf::FloatRect ObjetoGrafico::getHitbox() const {
    return hitbox;
}
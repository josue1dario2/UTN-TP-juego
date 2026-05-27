#include "Personaje.h"

Personaje::Personaje() {
    // Valores por defecto por ahora
    idPersonaje = 0;
    nombre = "Superviviente";
    vidaMax = 100.f;
    vidaActual = 100.f;
    armaduraMax = 50.f;
    armaduraActual = 50.f;
    velocidad = 200.f;
    habilidad = "Ninguna";
    mapaColision = nullptr;

    cargarTextura("assets/jugador2.png");

    setearTamanioSprite(13, 16);
    centrarOrigen();

    sprite.setScale(3.f,3.f);
    sprite.setPosition(1920.f, 1080.f);
}

void Personaje::setMapaColision(const sf::Image* mapa) {
    mapaColision = mapa;
}

bool Personaje::esPosicionValida(float x, float y) const {
    if (!mapaColision) return true;

    sf::Vector2u limite = mapaColision->getSize();

    // Si la posición está fuera de los límites de la imagen, no es válida
    if (x < 0.f || y < 0.f || x >= static_cast<float>(limite.x) || y >= static_cast<float>(limite.y)) {
        return false;
    }

    // Obtener color del píxel del mapa
    sf::Color color = mapaColision->getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y));

    // Si es negro (0,0,0), blanco (255,255,255), fucsia estándar (255,0,255) o el fucsia exacto del mapa (238,0,255)
    if ((color.r == 0 && color.g == 0 && color.b == 0) || 
        (color.r == 255 && color.g == 255 && color.b == 255) ||
        (color.r == 255 && color.g == 0 && color.b == 255) ||
        (color.r == 238 && color.g == 0 && color.b == 255)) {
        return false;
    }

    return true;
}

void Personaje::cargarAtributos(int id, std::string nom, float vida, float armadura, float vel, std::string hab) {
    idPersonaje = id;
    nombre = nom;
    vidaMax = vida;
    vidaActual = vida;
    armaduraMax = armadura;
    armaduraActual = armadura;
    velocidad = vel;
    habilidad = hab;
}

void Personaje::controlar(float movimiento) {
    sf::Vector2f posActual = sprite.getPosition();
    
    // Caja de colisión adaptada a la escala del personaje (13x16 escalado x3)
    // Ancho total aproximado: 39px, Alto total: 48px.
    // Usamos márgenes de seguridad para las esquinas (deltaX = 12px, deltaY = 18px desde el centro)
    float deltaX = 12.f;
    float deltaY = 18.f;

    // Movimiento vertical W (Arriba)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        float nuevaY = posActual.y - movimiento;
        if (esPosicionValida(posActual.x - deltaX, nuevaY - deltaY) && 
            esPosicionValida(posActual.x + deltaX, nuevaY - deltaY)) {
            sprite.move(0.f, -movimiento);
        }
    }
    // Movimiento vertical S (Abajo)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        float nuevaY = posActual.y + movimiento;
        if (esPosicionValida(posActual.x - deltaX, nuevaY + deltaY) && 
            esPosicionValida(posActual.x + deltaX, nuevaY + deltaY)) {
            sprite.move(0.f, movimiento);
        }
    }
    
    // Volver a obtener posición tras movimiento vertical para evaluar el horizontal de forma fluida
    posActual = sprite.getPosition();

    // Movimiento horizontal A (Izquierda)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        float nuevaX = posActual.x - movimiento;
        if (esPosicionValida(nuevaX - deltaX, posActual.y - deltaY) && 
            esPosicionValida(nuevaX - deltaX, posActual.y + deltaY)) {
            sprite.move(-movimiento, 0.f);
        }
    }
    // Movimiento horizontal D (Derecha)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        float nuevaX = posActual.x + movimiento;
        if (esPosicionValida(nuevaX + deltaX, posActual.y - deltaY) && 
            esPosicionValida(nuevaX + deltaX, posActual.y + deltaY)) {
            sprite.move(movimiento, 0.f);
        }
    }
}

void Personaje::actualizar(float deltaTime) {
    // lógica adicional para el personaje, como animaciones o habilidades
    float movimiento = velocidad * deltaTime;
    controlar(movimiento);

}
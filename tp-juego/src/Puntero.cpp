#include "Puntero.h"

Puntero::Puntero() : ObjetoGrafico() {
    // 1. Cargar la imagen del asset desde el archivo
    cargarTextura("assets/mira.png");
    centrarOrigen(); // Centrar el origen para la rotación

    // Ocultar la hitbox de depuración para que no dibuje un recuadro rojo alrededor de la mira
    mostrarHitbox = false;
}

void Puntero::actualizar(sf::RenderWindow& ventana, float deltaTime) {
    // 1. Obtener la posición del cursor en píxeles
    sf::Vector2i posMouse = sf::Mouse::getPosition(ventana);

    // 2. Traducir a coordenadas del mundo del juego
    sf::Vector2f posMundo = ventana.mapPixelToCoords(posMouse);

    // 3. Colocar el centro de la mira en el cursor
    setPosicionCentrado(posMundo.x, posMundo.y);

    // 4. Rotar el puntero
    sprite.rotate(50.f * deltaTime);
}

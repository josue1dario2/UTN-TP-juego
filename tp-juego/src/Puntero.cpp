#include "Puntero.h"

Puntero::Puntero() : ObjetoGrafico() {
    // 1. Cargar la imagen del asset desde el archivo
    sf::Image imagen;
    if (imagen.loadFromFile("assets/mira.png")) {
        
        // 2. Recorrer la imagen píxel por píxel y hacer transparente cualquier color muy oscuro (umbral < 40).
        // Esto elimina cualquier ruido de compresión, sombreados o bordes oscuros de forma matemática perfecta.
        sf::Vector2u tamano = imagen.getSize();
        for (unsigned int x = 0; x < tamano.x; ++x) {
            for (unsigned int y = 0; y < tamano.y; ++y) {
                sf::Color pixel = imagen.getPixel(x, y);
                
                // Si el color es muy oscuro (negro, gris muy oscuro o sombras de compresión)
                if (pixel.r < 40 && pixel.g < 40 && pixel.b < 40) {
                    imagen.setPixel(x, y, sf::Color(0, 0, 0, 0)); // Hacer 100% transparente
                }
            }
        }
        
        // 3. Crear la textura a partir de la imagen filtrada
        if (textura.loadFromImage(imagen)) {
            sprite.setTexture(textura);
            centrarOrigen(); // Centrar el origen para la rotación

            // 4. Escalar dinámicamente a la mitad del tamaño (32x32 píxeles)
            sf::FloatRect bounds = sprite.getLocalBounds();
            if (bounds.width > 0.f) {
                float escala = 32.f / bounds.width; // 32 píxeles de ancho
                escalarSprite(escala, escala);
            }
        }
    }
    
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

    // 4. Rotar el puntero al DOBLE de velocidad (180 grados por segundo)
    sprite.rotate(180.f * deltaTime);
}

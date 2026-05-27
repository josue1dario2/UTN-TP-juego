// gen_assets.cpp - Generador de sprites animados en C++ puro
// Este tool genera sprites animados para el juego

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Procesa un sprite individual
bool processSprite(const std::string& filename, const std::string& outFilename, int numFrames = 4) {
    sf::Image img;
    if (!img.loadFromFile(filename)) {
        std::cerr << "Error cargando: " << filename << std::endl;
        return false;
    }

    // Hacer transparente el fondo blanco/claro
    sf::Vector2u size = img.getSize();
    for (unsigned int y = 0; y < size.y; y++) {
        for (unsigned int x = 0; x < size.x; x++) {
            sf::Color pixel = img.getPixel(x, y);
            
            // Si es color muy claro (ruido de fondo DALL-E)
            if (pixel.r > 200 && pixel.g > 200 && pixel.b > 200) {
                pixel.a = 0;
                img.setPixel(x, y, pixel);
            }
        }
    }

    // Encontrar bounding box del personaje
    int minX = size.x, minY = size.y;
    int maxX = 0, maxY = 0;
    
    for (unsigned int y = 0; y < size.y; y++) {
        for (unsigned int x = 0; x < size.x; x++) {
            if (img.getPixel(x, y).a > 0) {
                minX = std::min(minX, (int)x);
                minY = std::min(minY, (int)y);
                maxX = std::max(maxX, (int)x);
                maxY = std::max(maxY, (int)y);
            }
        }
    }

    if (minX > maxX) {
        std::cerr << "Imagen vacia: " << filename << std::endl;
        return false;
    }

    // Padding
    int pad = 10;
    minX = std::max(0, minX - pad);
    minY = std::max(0, minY - pad);
    maxX = std::min((int)size.x, maxX + pad);
    maxY = std::min((int)size.y, maxY + pad);

    int charWidth = maxX - minX;
    int charHeight = maxY - minY;

    // Crear spritesheet (animation frames)
    int sheetWidth = charWidth * numFrames;
    int sheetHeight = charHeight;
    sf::Image sheet;
    sheet.create(sheetWidth, sheetHeight, sf::Color::Transparent);

    for (int i = 0; i < numFrames; i++) {
        // Offset vertical para simular caminar
        int yOffset = 0;
        if (i == 1) yOffset = -4;
        if (i == 3) yOffset = 4;

        // Copiar frame con offset
        for (int y = 0; y < charHeight; y++) {
            for (int x = 0; x < charWidth; x++) {
                int srcY = y - yOffset;
                if (srcY >= 0 && srcY < charHeight) {
                    sf::Color pixel = img.getPixel(minX + x, minY + srcY);
                    sheet.setPixel(i * charWidth + x, y, pixel);
                }
            }
        }
    }

    // Guardar
    if (sheet.saveToFile(outFilename)) {
        std::cout << "Guardado: " << outFilename << std::endl;
        return true;
    }
    return false;
}

int main() {
    std::cout << "=== Generador de Sprites Animados ===" << std::endl;
    
    // Los archivos deben estar en textures/ dentro del directorio actual
    processSprite("textures/player_tank.png", "textures/player_tank_walk.png");
    processSprite("textures/player_scout.png", "textures/player_scout_walk.png");
    processSprite("textures/player_medic.png", "textures/player_medic_walk.png");
    processSprite("textures/zombie_normal.png", "textures/zombie_walk.png");
    
    std::cout << "=== Sprites generados ===" << std::endl;
    return 0;
}
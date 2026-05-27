// Mapa.cpp - Sistema de mapa tile-based con obstáculos

#include "Mapa.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

Mapa::Mapa() : columnas(COLUMNAS_TILES), filas(FILAS_TILES) {
    // Inicializar matriz de tiles (todo piso por defecto)
    tiles.resize(columnas);
    for (int x = 0; x < columnas; x++) {
        tiles[x].resize(filas, PISO);
    }
    
    // Cargar textura de la pared/obstáculo
    if (texturaPared.loadFromFile("assets/textures/wall.png")) {
        spritePared.setTexture(texturaPared);
        sf::Vector2u size = texturaPared.getSize();
        if (size.x > 0 && size.y > 0) {
            spritePared.setScale(static_cast<float>(TAMANO_TILE) / size.x, static_cast<float>(TAMANO_TILE) / size.y);
        }
    }
}

Mapa::~Mapa() {}

bool Mapa::cargarDesdeArchivo(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[Mapa] ERROR: No se pudo abrir archivo de mapa: " << filepath << std::endl;
        return false;
    }
    
    std::string line;
    int fila = 0;
    
    while (std::getline(file, line) && fila < filas) {
        std::istringstream stream(line);
        std::string valor;
        int columna = 0;
        
        while (std::getline(stream, valor, ',') && columna < columnas) {
            try {
                tiles[columna][fila] = std::stoi(valor);
            } catch (...) {
                tiles[columna][fila] = PISO;
            }
            columna++;
        }
        fila++;
    }
    
    std::cout << "[Mapa] Mapa cargado desde: " << filepath << std::endl;
    return true;
}

bool Mapa::estaEnLimites(int tileX, int tileY) const {
    return tileX >= 0 && tileX < columnas && tileY >= 0 && tileY < filas;
}

bool Mapa::esBloqueante(int tileX, int tileY) const {
    if (!estaEnLimites(tileX, tileY)) return true;
    return tiles[tileX][tileY] == PARED;
}

bool Mapa::esBloqueante(sf::Vector2f posicion) const {
    int tileX = static_cast<int>(posicion.x / TAMANO_TILE);
    int tileY = static_cast<int>(posicion.y / TAMANO_TILE);
    return esBloqueante(tileX, tileY);
}

TipoTile Mapa::getTileEn(float x, float y) const {
    int tileX = static_cast<int>(x / TAMANO_TILE);
    int tileY = static_cast<int>(y / TAMANO_TILE);
    
    if (!estaEnLimites(tileX, tileY)) return PARED;
    return static_cast<TipoTile>(tiles[tileX][tileY]);
}

bool Mapa::esPosicionValida(float x, float y) const {
    int tileX = static_cast<int>(x / TAMANO_TILE);
    int tileY = static_cast<int>(y / TAMANO_TILE);
    
    // Verificar que esté dentro del mapa y no sea pared
    if (tileX < 2 || tileX >= columnas - 2 || tileY < 2 || tileY >= filas - 2) {
        return false;
    }
    return !esBloqueante(tileX, tileY);
}

sf::Vector2f Mapa::getPosicionSpawnValida() const {
    // Buscar una posición válida intentando múltiples veces
    for (int intento = 0; intento < 100; intento++) {
        int tileX = 2 + std::rand() % (columnas - 4);
        int tileY = 2 + std::rand() % (filas - 4);
        
        if (!esBloqueante(tileX, tileY)) {
            float x = static_cast<float>(tileX * TAMANO_TILE + TAMANO_TILE / 2);
            float y = static_cast<float>(tileY * TAMANO_TILE + TAMANO_TILE / 2);
            return sf::Vector2f(x, y);
        }
    }
    
    // Si no encuentra, retornar centro del mapa
    return sf::Vector2f(ANCHO_MAPA_PIXELES / 2.f, ALTO_MAPA_PIXELES / 2.f);
}

void Mapa::render(sf::RenderWindow& ventana, sf::Vector2f camara) {
    // Calcular tiles visibles basado en la cámara
    int tileInicialX = static_cast<int>(camara.x / TAMANO_TILE) - 1;
    int tileInicialY = static_cast<int>(camara.y / TAMANO_TILE) - 1;
    int tileFinalX = tileInicialX + (1280 / TAMANO_TILE) + 3;
    int tileFinalY = tileInicialY + (720 / TAMANO_TILE) + 3;
    
    // Dibujar solo los tiles visibles
    for (int x = tileInicialX; x < tileFinalX && x < columnas; x++) {
        for (int y = tileInicialY; y < tileFinalY && y < filas; y++) {
            if (x < 0 || y < 0) continue;
            
            if (tiles[x][y] == PARED) {
                // Dibujar obstáculo (muro de ladrillos/caja)
                spritePared.setPosition(static_cast<float>(x * TAMANO_TILE), static_cast<float>(y * TAMANO_TILE));
                ventana.draw(spritePared);
            }
        }
    }
}

void Mapa::generarMapaAleatorio() {
    // Sembrar random
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    // Crear bordes de paredes
    for (int x = 0; x < columnas; x++) {
        for (int y = 0; y < filas; y++) {
            if (x == 0 || x == columnas - 1 || y == 0 || y == filas - 1) {
                tiles[x][y] = PARED;
            } else {
                tiles[x][y] = PISO;
            }
        }
    }
    
    // Generar obstáculos aleatorios (10% de probabilidad para que no sea muy denso)
    for (int x = 2; x < columnas - 2; x++) {
        for (int y = 2; y < filas - 2; y++) {
            // No generar paredes muy cerca del centro (zona segura inicial)
            float distCentro = std::sqrt(std::pow(x - columnas/2.f, 2.f) + std::pow(y - filas/2.f, 2.f));
            if (distCentro < 5.f) continue;
            
            if (std::rand() % 100 < 10) {
                tiles[x][y] = PARED;
            }
        }
    }
    
    std::cout << "[Mapa] Mapa aleatorio generado con muros" << std::endl;
}
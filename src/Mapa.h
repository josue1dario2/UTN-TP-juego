#pragma once
// Mapa.h - Sistema de mapa tile-based con obstáculos

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

constexpr int TAMANO_TILE = 64;
constexpr int COLUMNAS_TILES = 30;
constexpr int FILAS_TILES = 20;
constexpr int ANCHO_MAPA_PIXELES = COLUMNAS_TILES * TAMANO_TILE;
constexpr int ALTO_MAPA_PIXELES = FILAS_TILES * TAMANO_TILE;

enum TipoTile {
    PISO = 0,
    PARED = 1
};

class Mapa {
public:
    Mapa();
    ~Mapa();

    // Cargar mapa desde archivo CSV
    bool cargarDesdeArchivo(const std::string& filepath);
    
    // Verificar si un tile es bloqueante
    bool esBloqueante(int tileX, int tileY) const;
    bool esBloqueante(sf::Vector2f posicion) const;
    
    // Obtener tipo de tile en coordenadas del mundo
    TipoTile getTileEn(float x, float y) const;
    
    // Obtener tamaño del mapa en tiles
    int getColumnas() const { return columnas; }
    int getFilas() const { return filas; }
    
    // Verificar si una posición es válida para spawn
    bool esPosicionValida(float x, float y) const;
    
    // Obtener una posición válida aleatoria para spawn
    sf::Vector2f getPosicionSpawnValida() const;
    
    // Renderizar el mapa con la cámara
    void render(sf::RenderWindow& ventana, sf::Vector2f camara);
    
    // Generar mapa aleatorio simple (para testing)
    void generarMapaAleatorio();

private:
    int columnas;
    int filas;
    std::vector<std::vector<int>> tiles;
    
    sf::Texture texturaMapa;
    sf::Sprite spriteMapa;
    
    sf::Texture texturaPared;
    sf::Sprite spritePared;
    
    // Cargar textura del tilemap
    bool cargarTextura();
    
    // Verificar límites del mapa
    bool estaEnLimites(int tileX, int tileY) const;
};
#pragma once

// ============================================================================
// GestorRecursos.h - Gestor centralizado de recursos del juego
// ============================================================================
// Este módulo implementa el patrón "Resource Manager" para cargar y cachear
// todos los recursos del juego (texturas, fuentes, sonidos) en un solo lugar.
// Ventaja: evita cargar el mismo archivo múltiples veces y centraliza el
// manejo de errores de carga.
// ============================================================================

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

class GestorRecursos {
public:
    // ---- Métodos para texturas ----

    // Carga una textura desde un archivo y la almacena con un nombre clave.
    // Retorna true si la carga fue exitosa, false si falló (sin crashear).
    bool cargarTextura(const std::string& name, const std::string& filepath);

    // Obtiene una referencia a una textura previamente cargada.
    // Lanza std::runtime_error si la textura no existe en el mapa.
    sf::Texture& getTextura(const std::string& name);

    // Verifica si una textura con ese nombre ya fue cargada.
    bool tieneTextura(const std::string& name) const;

    // ---- Métodos para fuentes ----

    // Carga una fuente tipográfica desde un archivo .ttf o .otf.
    bool cargarFuente(const std::string& name, const std::string& filepath);

    // Obtiene referencia a una fuente cargada. Lanza excepción si no existe.
    sf::Font& getFuente(const std::string& name);

    // Verifica si una fuente con ese nombre ya fue cargada.
    bool tieneFuente(const std::string& name) const;

    // ---- Métodos para sonidos ----

    // Carga un buffer de sonido desde un archivo .wav o .ogg.
    bool cargarBufferSonido(const std::string& name, const std::string& filepath);

    // Obtiene referencia a un SoundBuffer cargado. Lanza excepción si no existe.
    sf::SoundBuffer& getBufferSonido(const std::string& name);

    // Verifica si un buffer de sonido con ese nombre ya fue cargado.
    bool tieneBufferSonido(const std::string& name) const;

private:
    // Mapas que almacenan los recursos usando std::string como clave.
    // std::map mantiene los recursos ordenados por nombre para búsqueda eficiente.
    std::map<std::string, sf::Texture> textures;       // Mapa de texturas
    std::map<std::string, sf::Font> fonts;              // Mapa de fuentes
    std::map<std::string, sf::SoundBuffer> soundBuffers; // Mapa de sonidos
};
#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <vector>

class SoundManager {
private:
    static std::map<std::string, sf::SoundBuffer> buffers;
    static std::vector<sf::Sound> activeSounds;

public:
    static bool cargarSonidos();
    static void play(const std::string& nombre);
    static void limpiarSonidosMuertos();
};

#include "../include/SoundManager.h"
#include <iostream>
#include <algorithm>

std::map<std::string, sf::SoundBuffer> SoundManager::buffers;
std::vector<sf::Sound> SoundManager::activeSounds;

bool SoundManager::cargarSonidos() {
    std::vector<std::pair<std::string, std::string>> sonidosParaCargar = {
        {"bomba_explosion", "assets/sonidos/bomba_explosion.wav"},
        {"bomba_previo", "assets/sonidos/bomba_previo.wav"},
        {"click_boton", "assets/sonidos/click_boton.wav"},
        {"colocar_bomba", "assets/sonidos/colocar_bomba.wav"},
        {"cuchillo_aire", "assets/sonidos/cuchillo_aire.wav"},
        {"cuchillo_impacto", "assets/sonidos/cuchillo_impacto.wav"},
        {"disparo_escopeta", "assets/sonidos/disparo_escopeta.wav"},
        {"flecha", "assets/sonidos/flecha.wav"},
        {"muerte", "assets/sonidos/muerte.wav"},
        {"pistola", "assets/sonidos/pistola.wav"},
        {"recargar_pistola", "assets/sonidos/recargar_pistola.wav"},
        {"recargar_rifle", "assets/sonidos/recargar_rifle.wav"},
        {"recargar_scopeta", "assets/sonidos/recargar_scopeta.wav"},
        {"rifle", "assets/sonidos/rifle.wav"},
        {"seleccion", "assets/sonidos/seleccion.wav"}
    };

    bool todoCorrecto = true;
    for (const auto& par : sonidosParaCargar) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(par.second)) {
            buffers[par.first] = buffer;
            std::cout << "Sonido cargado correctamente: " << par.first << " desde " << par.second << std::endl;
        } else {
            std::cerr << "ERROR: No se pudo cargar el sonido: " << par.first << " desde " << par.second << std::endl;
            todoCorrecto = false;
        }
    }
    return todoCorrecto;
}

void SoundManager::play(const std::string& nombre) {
    auto it = buffers.find(nombre);
    if (it != buffers.end()) {
        // Limpiamos sonidos antiguos para no saturar memoria
        limpiarSonidosMuertos();

        // Creamos un nuevo sf::Sound y lo configuramos
        activeSounds.emplace_back(it->second);
        activeSounds.back().play();
    } else {
        std::cerr << "Advertencia: Se intento reproducir un sonido inexistente: " << nombre << std::endl;
    }
}

void SoundManager::limpiarSonidosMuertos() {
    activeSounds.erase(
        std::remove_if(activeSounds.begin(), activeSounds.end(),
            [](const sf::Sound& s) { return s.getStatus() == sf::SoundSource::Status::Stopped; }),
        activeSounds.end()
    );
}

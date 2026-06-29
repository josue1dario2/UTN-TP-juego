#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Config {
    // Configuración de Pantalla
    namespace Ventana {
        inline constexpr float Ancho = 1280.f;
        inline constexpr float Alto = 720.f;
        inline const std::string Titulo = "The Last Squad";
    }

    // Configuración de la Interfaz y Botones
    namespace UI {
        inline constexpr float BotonAncho = 250.f;
        inline constexpr float BotonAlto = 50.f;
        
        // Colores del tema arcade
        inline const sf::Color ColorBotonNormal(25, 25, 25, 220);
        inline const sf::Color ColorBotonHover(100, 20, 20);
        inline const sf::Color ColorBordeNormal(140, 50, 30);
        inline const sf::Color ColorBordeHover(230, 140, 40);
        inline const sf::Color ColorTextoNormal(240, 235, 225);
        inline const sf::Color ColorTextoHover(255, 215, 0);
    }

    // Balance de Gameplay
    namespace Gameplay {
        inline constexpr float VelocidadJugadorDefault = 250.f;
        inline constexpr float VidaMaxDefault = 100.f;
        inline constexpr float ArmaduraMaxDefault = 50.f;
        inline constexpr int ZombiesIniciales = 15;
        inline constexpr int ZombiesPorOleadaDificultad = 7;
        
        inline constexpr float TiempoDescanso = 15.f;        // Los 15 segundos de descanso
        inline constexpr float TiempoMaxOleada = 90.f;      // Tiempo límite para forzar la ronda
        inline constexpr float FrecuenciaSpawn = 0.5f;       // Frecuencia de aparición de zombies
    }
}

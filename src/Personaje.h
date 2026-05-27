#pragma once

#include <string>

// ============================================================================
// Personaje.h - Clase de datos de personaje jugable
// ============================================================================
// Representa los atributos base de un personaje extraídos del archivo
// Personajes.dat. No tiene lógica de juego, solo datos puros.
//
// Cada personaje tiene:
//   - id: identificador único
//   - nombre: nombre del personaje
//   - vidaInicial: puntos de vida base
//   - armaduraInicial: puntos de armadura base
//   - velocidadInicial: velocidad de movimiento (px/segundo)
//   - habilidad: nombre de la habilidad especial
// ============================================================================

class Personaje {
private:
    int id;
    std::string nombre;
    int vidaInicial;
    int armaduraInicial;
    int velocidadInicial;
    std::string habilidad;

public:
    Personaje();
    Personaje(int id, const std::string& nombre, int vida, int armadura, 
              int velocidad, const std::string& habilidad);

    int getId() const;
    std::string getNombre() const;
    int getVidaInicial() const;
    int getArmaduraInicial() const;
    int getVelocidadInicial() const;
    std::string getHabilidad() const;

    void setId(int valor);
    void setNombre(const std::string& valor);
    void setVidaInicial(int valor);
    void setArmaduraInicial(int valor);
    void setVelocidadInicial(int valor);
    void setHabilidad(const std::string& valor);
};
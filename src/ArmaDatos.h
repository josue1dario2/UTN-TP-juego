#pragma once

#include <string>

// ============================================================================
// ArmaDatos.h - Clase de datos de arma del juego
// ============================================================================
// Representa los atributos de un arma extraídos del archivo Armas.dat.
// No tiene lógica de combate, solo datos puros para crear un Arma.
//
// Cada arma tiene:
//   - id: identificador único
//   - nombre: nombre del arma
//   - danoBase: daño por disparo
//   - cadencia: segundos entre disparos
//   - alcance: alcance máximo en píxeles
//   - tipo: tipo de arma (pistola, escopeta, etc.)
//   - tamanoMagazine: balas por cargador
//   - municionTotal: munición total disponible
// ============================================================================

class ArmaDatos {
private:
    int id;
    std::string nombre;
    int danoBase;
    float cadencia;
    float alcance;
    std::string tipo;
    int tamanoMagazine;
    int municionTotal;

public:
    ArmaDatos();
    ArmaDatos(int id, const std::string& nombre, int dano, float cadencia,
              float alcance, const std::string& tipo, int tamanoMag, int municion);

    int getId() const;
    std::string getNombre() const;
    int getDanoBase() const;
    float getCadencia() const;
    float getAlcance() const;
    std::string getTipo() const;
    int getTamanoMagazine() const;
    int getMunicionTotal() const;

    void setId(int valor);
    void setNombre(const std::string& valor);
    void setDanoBase(int valor);
    void setCadencia(float valor);
    void setAlcance(float valor);
    void setTipo(const std::string& valor);
    void setTamanoMagazine(int valor);
    void setMunicionTotal(int valor);
};
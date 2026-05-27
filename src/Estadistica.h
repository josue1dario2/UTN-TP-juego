#pragma once

#include <string>

// ============================================================================
// Estadistica.h - Clase de datos para estadísticas de partidas
// ============================================================================
// Representa el récord de una partida jugado, extraído del archivo
// Estadisticas.dat. No tiene lógica de juego, solo datos puros.
//
// Cada entrada de estadísticas tiene:
//   - nombreJugador: nombre del jugador
//   - idPersonaje: ID del personaje utilizado
//   - puntuacion: puntaje final obtenido
//   - tiempoJugado: tiempo total de la partida en segundos
// ============================================================================

class Estadistica {
private:
    std::string nombreJugador;
    int idPersonaje;
    int puntuacion;
    int tiempoJugado;

public:
    Estadistica();
    Estadistica(const std::string& nombre, int idPers, int puntos, int tiempo);

    std::string getNombreJugador() const;
    int getIdPersonaje() const;
    int getPuntuacion() const;
    int getTiempoJugado() const;

    void setNombreJugador(const std::string& valor);
    void setIdPersonaje(int valor);
    void setPuntuacion(int valor);
    void setTiempoJugado(int valor);

    // Convierte a formato CSV para guardar en archivo
    std::string aCSV() const;

    // Crea una Estadistica desde una línea CSV
    static Estadistica desdeCSV(const std::string& linea);
};
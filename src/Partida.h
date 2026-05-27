#pragma once

#include <string>

// ============================================================================
// Partida.h - Clase que representa el estado de una partida guardada
// ============================================================================
// Gestiona el estado completo de una sesión de juego para poder
// guardarla y cargarla. Incluye oleada actual, puntos y tiempo.
//
// Para guardar en archivo se usan los métodos guardar() y cargar(),
// que trabajan con PartidaActual.dat.
// ============================================================================

class Partida {
private:
    int idPartida;
    int oleadaActual;
    int puntos;
    int tiempoJugado;
    int idPersonaje;
    std::string nombrePersonaje;

public:
    Partida();
    Partida(int id, int oleada, int puntos, int tiempo, 
            int idPers, const std::string& nombrePers);

    int getIdPartida() const;
    int getOleadaActual() const;
    int getPuntos() const;
    int getTiempoJugado() const;
    int getIdPersonaje() const;
    std::string getNombrePersonaje() const;

    void setIdPartida(int valor);
    void setOleadaActual(int valor);
    void setPuntos(int valor);
    void setTiempoJugado(int valor);
    void setIdPersonaje(int valor);
    void setNombrePersonaje(const std::string& valor);

    void iniciar();
    void actualizar(int oleada, int puntos, int tiempo);
    void finalizar();
    bool estaVacia() const;

    // Serialización para guardar/cargar
    std::string aCSV() const;
    static Partida desdeCSV(const std::string& linea);
};
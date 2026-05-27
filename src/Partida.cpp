#include "Partida.h"
#include <sstream>

Partida::Partida()
    : idPartida(0), oleadaActual(0), puntos(0), 
      tiempoJugado(0), idPersonaje(0), nombrePersonaje("") {
}

Partida::Partida(int id, int oleada, int puntos, int tiempo,
                int idPers, const std::string& nombrePers)
    : idPartida(id), oleadaActual(oleada), puntos(puntos),
      tiempoJugado(tiempo), idPersonaje(idPers), nombrePersonaje(nombrePers) {
}

int Partida::getIdPartida() const {
    return idPartida;
}

int Partida::getOleadaActual() const {
    return oleadaActual;
}

int Partida::getPuntos() const {
    return puntos;
}

int Partida::getTiempoJugado() const {
    return tiempoJugado;
}

int Partida::getIdPersonaje() const {
    return idPersonaje;
}

std::string Partida::getNombrePersonaje() const {
    return nombrePersonaje;
}

void Partida::setIdPartida(int valor) {
    idPartida = valor;
}

void Partida::setOleadaActual(int valor) {
    oleadaActual = valor;
}

void Partida::setPuntos(int valor) {
    puntos = valor;
}

void Partida::setTiempoJugado(int valor) {
    tiempoJugado = valor;
}

void Partida::setIdPersonaje(int valor) {
    idPersonaje = valor;
}

void Partida::setNombrePersonaje(const std::string& valor) {
    nombrePersonaje = valor;
}

void Partida::iniciar() {
    idPartida = 1;
    oleadaActual = 1;
    puntos = 0;
    tiempoJugado = 0;
}

void Partida::actualizar(int oleada, int puntos, int tiempo) {
    this->oleadaActual = oleada;
    this->puntos = puntos;
    this->tiempoJugado = tiempo;
}

void Partida::finalizar() {
    // Se deja el estado tal cual para guardarlo
}

bool Partida::estaVacia() const {
    return idPartida == 0;
}

std::string Partida::aCSV() const {
    return std::to_string(idPartida) + "," +
           std::to_string(oleadaActual) + "," +
           std::to_string(puntos) + "," +
           std::to_string(tiempoJugado) + "," +
           std::to_string(idPersonaje) + "," +
           nombrePersonaje;
}

Partida Partida::desdeCSV(const std::string& linea) {
    std::istringstream stream(linea);
    std::string campo;
    Partida p;

    if (std::getline(stream, campo, ',')) {
        p.setIdPartida(std::stoi(campo));
    }
    if (std::getline(stream, campo, ',')) {
        p.setOleadaActual(std::stoi(campo));
    }
    if (std::getline(stream, campo, ',')) {
        p.setPuntos(std::stoi(campo));
    }
    if (std::getline(stream, campo, ',')) {
        p.setTiempoJugado(std::stoi(campo));
    }
    if (std::getline(stream, campo, ',')) {
        p.setIdPersonaje(std::stoi(campo));
    }
    if (std::getline(stream, campo, ',')) {
        p.setNombrePersonaje(campo);
    }

    return p;
}
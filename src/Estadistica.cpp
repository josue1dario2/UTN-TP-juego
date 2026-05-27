#include "Estadistica.h"
#include <sstream>
#include <algorithm>

Estadistica::Estadistica()
    : nombreJugador(""), idPersonaje(0), puntuacion(0), tiempoJugado(0) {
}

Estadistica::Estadistica(const std::string& nombre, int idPers, int puntos, int tiempo)
    : nombreJugador(nombre), idPersonaje(idPers), puntuacion(puntos), tiempoJugado(tiempo) {
}

std::string Estadistica::getNombreJugador() const {
    return nombreJugador;
}

int Estadistica::getIdPersonaje() const {
    return idPersonaje;
}

int Estadistica::getPuntuacion() const {
    return puntuacion;
}

int Estadistica::getTiempoJugado() const {
    return tiempoJugado;
}

void Estadistica::setNombreJugador(const std::string& valor) {
    nombreJugador = valor;
}

void Estadistica::setIdPersonaje(int valor) {
    idPersonaje = valor;
}

void Estadistica::setPuntuacion(int valor) {
    puntuacion = valor;
}

void Estadistica::setTiempoJugado(int valor) {
    tiempoJugado = valor;
}

std::string Estadistica::aCSV() const {
    return nombreJugador + "," +
           std::to_string(idPersonaje) + "," +
           std::to_string(puntuacion) + "," +
           std::to_string(tiempoJugado);
}

Estadistica Estadistica::desdeCSV(const std::string& linea) {
    std::istringstream stream(linea);
    std::string campo;
    Estadistica est;

    if (std::getline(stream, campo, ',')) {
        est.setNombreJugador(campo);
    }
    if (std::getline(stream, campo, ',')) {
        est.setIdPersonaje(std::stoi(campo));
    }
    if (std::getline(stream, campo, ',')) {
        est.setPuntuacion(std::stoi(campo));
    }
    if (std::getline(stream, campo, ',')) {
        est.setTiempoJugado(std::stoi(campo));
    }

    return est;
}
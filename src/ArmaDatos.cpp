#include "ArmaDatos.h"

ArmaDatos::ArmaDatos()
    : id(0), nombre(""), danoBase(0), cadencia(0.f), alcance(0.f),
      tipo(""), tamanoMagazine(0), municionTotal(0) {
}

ArmaDatos::ArmaDatos(int id, const std::string& nombre, int dano, float cadencia,
                    float alcance, const std::string& tipo, int tamanoMag, int municion)
    : id(id), nombre(nombre), danoBase(dano), cadencia(cadencia), alcance(alcance),
      tipo(tipo), tamanoMagazine(tamanoMag), municionTotal(municion) {
}

int ArmaDatos::getId() const {
    return id;
}

std::string ArmaDatos::getNombre() const {
    return nombre;
}

int ArmaDatos::getDanoBase() const {
    return danoBase;
}

float ArmaDatos::getCadencia() const {
    return cadencia;
}

float ArmaDatos::getAlcance() const {
    return alcance;
}

std::string ArmaDatos::getTipo() const {
    return tipo;
}

int ArmaDatos::getTamanoMagazine() const {
    return tamanoMagazine;
}

int ArmaDatos::getMunicionTotal() const {
    return municionTotal;
}

void ArmaDatos::setId(int valor) {
    id = valor;
}

void ArmaDatos::setNombre(const std::string& valor) {
    nombre = valor;
}

void ArmaDatos::setDanoBase(int valor) {
    danoBase = valor;
}

void ArmaDatos::setCadencia(float valor) {
    cadencia = valor;
}

void ArmaDatos::setAlcance(float valor) {
    alcance = valor;
}

void ArmaDatos::setTipo(const std::string& valor) {
    tipo = valor;
}

void ArmaDatos::setTamanoMagazine(int valor) {
    tamanoMagazine = valor;
}

void ArmaDatos::setMunicionTotal(int valor) {
    municionTotal = valor;
}
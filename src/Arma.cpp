// Arma.cpp - Sistema de armas

#include "Arma.h"
#include <algorithm>

Arma::Arma(const ArmaDatos& data)
    : nombre(data.getNombre()),
      danoBase(data.getDanoBase()),
      cadencia(data.getCadencia()),
      alcance(data.getAlcance()),
      tipo(data.getTipo()),
      balaActual(data.getTamanoMagazine()),
      tamanoMagazine(data.getTamanoMagazine()),
      balasReserva(data.getMunicionTotal() - data.getTamanoMagazine()),
      balasTotales(data.getMunicionTotal()),
      cooldownDisparo(0.f),
      tiempoRecarga(1.5f),
      timerRecarga(0.f),
      recargando(false)
{
    if (balasReserva < 0) {
        balaActual = data.getMunicionTotal();
        balasReserva = 0;
    }
}

bool Arma::puedeDisparar() const {
    return !recargando && balaActual > 0 && cooldownDisparo <= 0.f;
}

void Arma::disparar() {
    if (!puedeDisparar()) return;
    balaActual--;
    cooldownDisparo = cadencia;
}

void Arma::recargar() {
    if (recargando) return;
    if (balasReserva <= 0) return;
    if (balaActual >= tamanoMagazine) return;

    recargando = true;
    timerRecarga = tiempoRecarga;
}

void Arma::update(float dt) {
    if (cooldownDisparo > 0.f) {
        cooldownDisparo -= dt;
        if (cooldownDisparo < 0.f) {
            cooldownDisparo = 0.f;
        }
    }

    if (recargando) {
        timerRecarga -= dt;

        if (timerRecarga <= 0.f) {
            recargando = false;
            int balasNecesarias = tamanoMagazine - balaActual;
            int balasCargar = std::min(balasNecesarias, balasReserva);
            balaActual += balasCargar;
            balasReserva -= balasCargar;
        }
    }
}

int Arma::getDano() const {
    return danoBase;
}

float Arma::getAlcance() const {
    return alcance;
}

const std::string& Arma::getNombre() const {
    return nombre;
}

int Arma::getBalasEnMagazine() const {
    return balaActual;
}

int Arma::getBalasTotales() const {
    return balasReserva;
}

bool Arma::estaRecargando() const {
    return recargando;
}

float Arma::getCadencia() const {
    return cadencia;
}

const std::string& Arma::getTipo() const {
    return tipo;
}
#include "Arma.h"
#include <cmath>
#include <iostream>

Arma::Arma() {
    idArma = 1;
    nombre = "Pistola de Supervivencia";
    danio = 35;
    alcance = 600.f;
    costo = 0.f;
    cargarTextura("assets/pistola.png");
    centrarOrigen();
    escalarSprite(2.f, 2.f);
}

Arma::Arma(int id, std::string nom, int dmg, float alc, float cost) {
    idArma = id;
    nombre = nom;
    danio = dmg;
    alcance = alc;
    costo = cost;
}

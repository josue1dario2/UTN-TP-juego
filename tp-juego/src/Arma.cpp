#include "Arma.h"
#include <cmath>
#include <iostream>

Arma::Arma() {
    idArma = 1;
    nombre = "Pistola de Supervivencia";
    danio = 35;
    alcance = 600.f;
    costo = 0.f;
}

Arma::Arma(int id, std::string nom, int dmg, float alc, float cost) {
    idArma = id;
    nombre = nom;
    danio = dmg;
    alcance = alc;
    costo = cost;
}

int Arma::getIdArma() const { return idArma; }
std::string Arma::getNombre() const { return nombre; }
int Arma::getDanio() const { return danio; }
float Arma::getAlcance() const { return alcance; }
float Arma::getCosto() const { return costo; }

void Arma::disparar(sf::Vector2f origen, sf::Vector2f destino) {
    // Calcular dirección del disparo
    sf::Vector2f dir = destino - origen;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.f) {
        dir /= len;
    } else {
        dir = sf::Vector2f(1.f, 0.f);
    }

    // Instanciar un Proyectil formal (que hereda de ObjetoGrafico y tiene Hitbox)
    proyectiles.push_back(Proyectil(origen, dir, alcance, 800.f));

    std::cout << nombre << " disparada! Objeto Proyectil lanzado hacia el mouse." << std::endl;
}

void Arma::recargar() {
    std::cout << "Recargando el arma: " << nombre << std::endl;
}

void Arma::actualizar(float deltaTime) {
    for (size_t i = 0; i < proyectiles.size(); ) {
        // Actualizar el proyectil
        proyectiles[i].actualizar(deltaTime);
        
        // Verificar colisión del proyectil contra el mapa de color
        proyectiles[i].verificarColisiones();

        // Si ya no está activo, eliminar de la lista
        if (!proyectiles[i].isActivo()) {
            proyectiles.erase(proyectiles.begin() + i);
            continue;
        }

        i++;
    }
}

void Arma::dibujar(sf::RenderWindow& ventana) {
    for (auto& proyectil : proyectiles) {
        proyectil.dibujar(ventana);
    }
}

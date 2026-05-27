#pragma once
#include <vector>
#include "Zombie.h"

class Oleada {
private:
    int numero;
    int enemigosTotales;
    int enemigosVivos;
    bool completada;

    // Vector de enemigos que pertenecen a esta oleada
    std::vector<Zombie> enemigos;

public:
    Oleada();
    Oleada(int numero, int enemigosTotales);

    // Getters y Setters
    int getNumero() const;
    void setNumero(int numero);

    int getEnemigosTotales() const;
    void setEnemigosTotales(int enemigosTotales);

    int getEnemigosVivos() const;
    void setEnemigosVivos(int enemigosVivos);

    bool isCompletada() const;
    void setCompletada(bool completada);

    const std::vector<Zombie>& getEnemigos() const;
    std::vector<Zombie>& getEnemigos();

    // Métodos del diagrama UML
    void iniciar();
    void actualizar();
};

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Proyectil.h"

class Arma : public ObjetoGrafico {
private:
    int idArma;
    std::string nombre;
    
    float tiempoDesdeUltimoDisparo;
    float tiempoRecarga;
    bool enRecarga;

    float cadencia;
    float danio;
    float alcance;
    float costo;
    int municionMaxima;
    int tamanioCargador;

    int municionActual;
    int municionEnCargador;
    bool desbloqueada;

    void disparoEscopeta(float deltaX, float deltaY, std::vector<Proyectil>& proyectiles, sf::Texture& texturaProyectil);

public:
    Arma();
    Arma(int id, std::string nombre, float cadencia, float danio, float alcance, float costo, int municionMaxima, int tamanioCargador);

    virtual void actualizar(float deltaTime, const sf::Vector2f &posicionMouse, const sf::Vector2f& posicionJugador, 
        std::vector<Proyectil>& proyectiles, sf::Texture& texturaProyectil);

    // Getters y Setters
    int getIdArma() const;
    std::string getNombre() const;
    float getDanio() const;
    float getAlcance() const;
    float getCosto() const;

    void setDesbloqueo(bool estado);
    void llenarMunicion();
    void recargar(int cantidad);

    int getMunicionActual() const { return municionActual; }
    int getMunicionEnCargador() const { return municionEnCargador; }
    int getTamanioCargador() const { return tamanioCargador; }
    int getMunicionMaxima() const { return municionMaxima; }

    bool getEnRecarga() const { return enRecarga; }
    bool getEstadoDesbloqueo() const { return desbloqueada; }

    bool estaDisponible() const; // Verifica si el arma está disponible para ser equipada
};

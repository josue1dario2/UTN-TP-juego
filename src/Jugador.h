#pragma once

#include "Entidad.h"
#include <string>
#include <vector>
#include "Personaje.h"

class Arma;

class Jugador : public Entidad {
public:
    Jugador(const Personaje& data, sf::Texture& texture);
    ~Jugador() override;

    void update(float deltaTime) override;
    void render(sf::RenderWindow& ventana) override;

    void apuntar(sf::Vector2f posMouse);
    Entidad* disparar(sf::Vector2f posMouse, sf::Texture& texturaBala);
    void cambiarArma(int indice);
    void recargar();
    void recibirDano(int cantidad) override;
    void agregarArma(Arma* arma);
    void agregarPuntos(int cantidad);

    int getPuntos() const;
    int getArmadura() const;
    int getArmaduraMaxima() const;
    std::string getNombre() const;
    Arma* getArmaActual() const;
    std::string getHabilidadEspecial() const;
    int getIdPersonaje() const;
    bool puedeUsarHabilidad() const;
    bool estaEscudoActivo() const;
    float getCooldownHabilidad() const;
    int getSaludMaxima() const;
    void aumentarSaludMaxima(int cantidad);
    void aumentarArmaduraMaxima(int cantidad);

    void activarHabilidad(sf::Vector2f direccion);

private:
    int idPersonaje;
    std::string nombre;
    std::string habilidadEspecial;

    int armadura;
    int armaduraMaxima;

    std::vector<Arma*> armas;
    int indiceArmaActual;

    int puntos;

    float timerHabilidad;
    float cooldownHabilidad;
    bool escudoActivo;
    float timerEscudo;
    float dashTimer;
    sf::Vector2f direccionDash;
    float regeneracionTimer;

    sf::Vector2f ultimaPosMouse;

    float timerCaminar;
    float escalaBase;
    int anchoFrame;
    int altoFrame;

    void mover(float deltaTime);
    void limitarVentana();
    void activarEscudo();
    void activarDash(sf::Vector2f direccion);
    void activarRegeneracion();
};
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Entidad.h"
#include "Arma.h"
#include "ObjetoMapa.h"
#include "Proyectil.h"
#include "archivoArma.h"


class Personaje : public Entidad {
private:
    // ------ VARIABLES ------
    int idPersonaje;
    std::string nombre;
    float armaduraMax;
    float armaduraActual;
    float cooldownHabilidad;
    std::string habilidad;


    sf::Vector2f posicionAnterior;
    float movimientoX;
    float movimientoY;

    // Arma equipada del jugador
    std::vector<Arma> inventarioArmas;
    int armaEquipada; // Índice del arma actualmente equipada en el inventario

    float tiempoHabilidad; //tiempo desde que se activo la habilidad
    bool habilidadActivada;
    bool habilidadDisponible;
    float multiplicadorZoom;
    
    sf::FloatRect zonaHabilidad;
    bool dashActivado;
    bool invulnerabilidad = false;
    
    // ----- FUNCIONES PRIVADAS -----
    void actualizarZonaHabilidad();

    void calcularDireccion(sf::Vector2f posicionMouse);

    void guardarPosicionAnterior();

    void volverPosicionAnteriorX();

    void volverPosicionAnteriorY();

    sf::Vector2f getPosicionAnterior() const { return posicionAnterior; }

    float getMovimientoX() const;

    float getMovimientoY() const;

    void setVelocidad(float velocidad);

    void elegirArma();
    
    void activarHabilidad(float deltaTime);

    void habilidadRecon(float deltaTime);

    void habilidadJoel();

    void habilidadGhost();

    void habilidadJohnWick();

    void habilidadSoldado();
    
    public:
    // ------ FUNCIONES PUBLICAS ------
    Personaje();
    
    Personaje(int id, int idArmaEspecial, std::string nombre, float vida, float armadura, float velocidad, float cooldownHabilidad);
    
    virtual void actualizar(float deltaTime, const std::vector<ObjetoMapa>& obstaculos, const std::vector<sf::FloatRect>& hitboxZombies, const sf::Vector2f &posicionMouse);
    
    Arma& getArma();
    
    float getMultiplicadorZoom();
    
    float getArmaduraActual() const { return armaduraActual; }
    
    float getArmaduraMax() const { return armaduraMax; }
    
    sf::FloatRect getZonaHabilidad() const;

    int getDireccion() const;

    bool habilidadActiva() const;
    
    void recibirDanio(float cantidad) override;

    int getId()const{ return idPersonaje;}

    bool esInvulnerable() const {return invulnerabilidad;}

};
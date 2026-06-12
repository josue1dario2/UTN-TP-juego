#pragma once
#include "Entidad.h"
#include "ObjetoMapa.h"
#include <vector>

class Zombie : public Entidad {
private:
  int tipo;
  int ataque;
  float tiempoDesdeUltimoAtaque;
  float cooldownAtaque;

public:
  Zombie();
  Zombie(int tipo, int vida, int ataque, float velocidad);

  // Getters y Setters
  int getTipo() const;
  void setTipo(int tipo);

  int getVida() const;
  void setVida(int vida);

  int getAtaque() const;
  void setAtaque(int ataque);

  float getVelocidad() const;
  void setVelocidad(float velocidad);

  // Métodos del diagrama UML
  void atacar();
  void quitarVida(int cantidad = 1);
  bool muerto() const;

  // Actualización con inteligencia de evasión, persecución y separación
  void actualizar(float deltaTime, const sf::FloatRect &hitboxJugador,
                  const std::vector<ObjetoMapa> &obstaculos,
                  const std::vector<Zombie> &todosLosZombies);

  bool puedeAtacar() const { return tiempoDesdeUltimoAtaque >= cooldownAtaque; }
  void reiniciarTiempoAtaque() { tiempoDesdeUltimoAtaque = 0.f; }
};

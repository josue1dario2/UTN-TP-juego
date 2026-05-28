#include "Proyectil.h"

Proyectil::Proyectil(sf::Vector2f posInicial, sf::Vector2f dir, float alc, float vel) : ObjetoGrafico() {
    direccion = dir;
    velocidad = vel;
    alcanceMax = alc;
    distanciaRecorrida = 0.f;
    activo = true;

    // Configurar la representación visual estándar de la bala
    formaBala.setRadius(5.f);
    formaBala.setFillColor(sf::Color::Yellow);
    formaBala.setOrigin(5.f, 5.f); // Centrado

    setPosicionCentrado(posInicial.x, posInicial.y);
    setHitbox(10.f, 10.f); // Hitbox pequeña para el proyectil
}

void Proyectil::actualizar(float deltaTime) {
    if (!activo) return;

    float paso = velocidad * deltaTime;
    mover(direccion.x * paso, direccion.y * paso);
    distanciaRecorrida += paso;

    // Si supera el alcance máximo, se desactiva
    if (distanciaRecorrida >= alcanceMax) {
        activo = false;
    }
}

void Proyectil::dibujar(sf::RenderWindow& ventana) {
    if (!activo) return;

    // Sincronizar la posición visual de la forma de la bala
    formaBala.setPosition(getPosicion());
    ventana.draw(formaBala);

    // Dibujar la hitbox de depuración si mostrarHitbox está activado en ObjetoGrafico
    ObjetoGrafico::dibujar(ventana);
}

void Proyectil::verificarColisiones() {

}

bool Proyectil::isActivo() const {
    return activo;
}

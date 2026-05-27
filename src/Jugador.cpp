// Jugador.cpp - Clase jugador

#include "Jugador.h"
#include "Arma.h"
#include "Bala.h"
#include "Mapa.h"
#include <cmath>

static constexpr float ANCHO_VENTANA = 1280.f;
static constexpr float ALTO_VENTANA = 720.f;

Jugador::Jugador(const Personaje& data, sf::Texture& texture)
    : Entidad(texture)
    , idPersonaje(data.getId())
    , nombre(data.getNombre())
    , habilidadEspecial(data.getHabilidad())
    , armadura(data.getArmaduraInicial())
    , armaduraMaxima(data.getArmaduraInicial())
    , indiceArmaActual(0)
    , puntos(0)
    , timerHabilidad(0.f)
    , cooldownHabilidad(10.f)
    , escudoActivo(false)
    , timerEscudo(0.f)
    , dashTimer(0.f)
    , regeneracionTimer(0.f)
    , ultimaPosMouse(0.f, 0.f)
    , timerCaminar(0.f)
    , escalaBase(1.0f)
{
    salud = data.getVidaInicial();
    saludMaxima = data.getVidaInicial();
    velocidad = static_cast<float>(data.getVelocidadInicial());

    sf::Vector2u texSize = texture.getSize();
    anchoFrame = texSize.x;
    altoFrame = texSize.y;
    
    sprite.setTextureRect(sf::IntRect(0, 0, anchoFrame, altoFrame));
    sprite.setOrigin(anchoFrame / 2.0f, altoFrame / 2.0f);

    if (anchoFrame > 0) {
        escalaBase = 120.f / static_cast<float>(anchoFrame);
        sprite.setScale(escalaBase, escalaBase);
    }

    setPosicion(sf::Vector2f(ANCHO_VENTANA / 2.f, ALTO_VENTANA / 2.f));
}

Jugador::~Jugador() {
    for (Arma* arma : armas) {
        delete arma;
        arma = nullptr;
    }
    armas.clear();
}

void Jugador::update(float deltaTime) {
    if (!vivo) return;

    mover(deltaTime);
    limitarVentana();

    for (Arma* arma : armas) {
        if (arma) {
            arma->update(deltaTime);
        }
    }

    if (timerHabilidad > 0.f) {
        timerHabilidad -= deltaTime;
        if (timerHabilidad < 0.f) {
            timerHabilidad = 0.f;
        }
    }

    // Update escudo
    if (escudoActivo) {
        timerEscudo -= deltaTime;
        if (timerEscudo <= 0.f) {
            escudoActivo = false;
        }
    }

    // Update dash
    if (dashTimer > 0.f) {
        dashTimer -= deltaTime;
        if (dashTimer <= 0.f) {
            dashTimer = 0.f;
            escudoActivo = false;
            velocidad = 180.f;
        }
    }

    // Update regeneracion
    if (regeneracionTimer > 0.f) {
        regeneracionTimer -= deltaTime;
        salud += 3;
        if (salud > saludMaxima) salud = saludMaxima;
        if (regeneracionTimer <= 0.f) {
            regeneracionTimer = 0.f;
        }
    }
}

void Jugador::render(sf::RenderWindow& ventana) {
    if (!vivo) return;
    ventana.draw(sprite);
}

void Jugador::apuntar(sf::Vector2f posMouse) {
    ultimaPosMouse = posMouse;
    float dx = posMouse.x - posicion.x;
    
    // En lugar de rotar 180 grados (lo que pone al personaje boca abajo), 
    // lo volteamos horizontalmente
    float stretch = sprite.getScale().y;
    if (dx < 0) {
        sprite.setScale(-escalaBase, stretch); // Mira izquierda
    } else {
        sprite.setScale(escalaBase, stretch); // Mira derecha
    }
}

Entidad* Jugador::disparar(sf::Vector2f posMouse, sf::Texture& texturaBala) {
    if (armas.empty()) return nullptr;

    Arma* armaActual = armas[indiceArmaActual];
    if (!armaActual) return nullptr;

    if (!armaActual->puedeDisparar()) {
        if (armaActual->getBalasEnMagazine() == 0 && !armaActual->estaRecargando()) {
            armaActual->recargar();
        }
        return nullptr;
    }

    armaActual->disparar();

    float dx = posMouse.x - posicion.x;
    float dy = posMouse.y - posicion.y;
    float magnitud = std::sqrt(dx * dx + dy * dy);

    if (magnitud == 0.f) {
        magnitud = 1.f;
    }

    sf::Vector2f direccion(dx / magnitud, dy / magnitud);

    float velocidadBala = 500.f;
    Bala* bala = new Bala(
        posicion,
        direccion,
        velocidadBala,
        armaActual->getDano(),
        armaActual->getAlcance(),
        texturaBala
    );

    return bala;
}

void Jugador::cambiarArma(int indice) {
    if (indice >= 0 && indice < static_cast<int>(armas.size())) {
        indiceArmaActual = indice;
    }
}

void Jugador::recargar() {
    if (!armas.empty() && armas[indiceArmaActual]) {
        armas[indiceArmaActual]->recargar();
    }
}

void Jugador::recibirDano(int cantidad) {
    if (!vivo) return;

    // Escudo Temporal bloquea todo el dano
    if (escudoActivo) {
        return;
    }

    if (armadura > 0) {
        int danoRestante = cantidad - armadura;
        armadura -= cantidad;

        if (armadura < 0) {
            armadura = 0;
            salud -= danoRestante;
        }
    } else {
        salud -= cantidad;
    }

    if (salud <= 0) {
        salud = 0;
        vivo = false;
    }
}

void Jugador::agregarArma(Arma* arma) {
    if (arma) {
        armas.push_back(arma);
    }
}

void Jugador::agregarPuntos(int cantidad) {
    puntos += cantidad;
}

int Jugador::getPuntos() const {
    return puntos;
}

int Jugador::getArmadura() const {
    return armadura;
}

int Jugador::getArmaduraMaxima() const {
    return armaduraMaxima;
}

std::string Jugador::getNombre() const {
    return nombre;
}

Arma* Jugador::getArmaActual() const {
    if (armas.empty()) return nullptr;
    return armas[indiceArmaActual];
}

std::string Jugador::getHabilidadEspecial() const {
    return habilidadEspecial;
}

int Jugador::getIdPersonaje() const {
    return idPersonaje;
}

bool Jugador::puedeUsarHabilidad() const {
    return timerHabilidad <= 0.f;
}

bool Jugador::estaEscudoActivo() const {
    return escudoActivo;
}

float Jugador::getCooldownHabilidad() const {
    if (timerHabilidad <= 0.f) return 0.f;
    return timerHabilidad / cooldownHabilidad;
}

int Jugador::getSaludMaxima() const {
    return saludMaxima;
}

void Jugador::aumentarSaludMaxima(int cantidad) {
    saludMaxima += cantidad;
    salud += cantidad;
}

void Jugador::aumentarArmaduraMaxima(int cantidad) {
    armaduraMaxima += cantidad;
    armadura += cantidad;
}

void Jugador::activarHabilidad(sf::Vector2f direccion) {
    if (!vivo || timerHabilidad > 0.f) return;

    if (habilidadEspecial == "Escudo Temporal") {
        activarEscudo();
    } else if (habilidadEspecial == "Dash Rapido") {
        activarDash(direccion);
    } else if (habilidadEspecial == "Regeneracion") {
        activarRegeneracion();
    }
}

void Jugador::activarEscudo() {
    escudoActivo = true;
    timerEscudo = 4.f;
    timerHabilidad = cooldownHabilidad;
}

void Jugador::activarDash(sf::Vector2f direccion) {
    if (direccion.x == 0.f && direccion.y == 0.f) {
        direccion = sf::Vector2f(1.f, 0.f);
    }
    float len = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
    if (len > 0.f) {
        direccion /= len;
    }
    direccionDash = direccion;
    dashTimer = 0.3f;
    timerHabilidad = cooldownHabilidad;
    escudoActivo = true;
    timerEscudo = 0.3f;
    velocidad = 800.f;
}

void Jugador::activarRegeneracion() {
    regeneracionTimer = 5.f;
    timerHabilidad = cooldownHabilidad;
}

void Jugador::mover(float deltaTime) {
    sf::Vector2f movimiento(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movimiento.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movimiento.y += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movimiento.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movimiento.x += 1.f;
    }

    float longitud = std::sqrt(movimiento.x * movimiento.x + movimiento.y * movimiento.y);
    if (longitud > 0.f) {
        movimiento.x /= longitud;
        movimiento.y /= longitud;

        // Calcular nueva posición
        sf::Vector2f nuevaPos(
            posicion.x + movimiento.x * velocidad * deltaTime,
            posicion.y + movimiento.y * velocidad * deltaTime
        );

        // Verificar colisión con mapa
        if (esPosicionValida(nuevaPos)) {
            posicion = nuevaPos;
        }

        // Animación de Caminar (Wobble/Rotación procedimental)
        timerCaminar += velocidad * deltaTime * 0.1f;
        float wobbleAngle = std::sin(timerCaminar) * 10.f; // Rotar levemente
        sprite.setRotation(wobbleAngle);
        
        // Efecto de rebote (squash vertical)
        float bounce = 1.0f + std::abs(std::sin(timerCaminar)) * 0.1f;
        
        // Mantener la dirección a la que mira
        float actualScaleX = (sprite.getScale().x < 0) ? -escalaBase : escalaBase;
        sprite.setScale(actualScaleX, escalaBase * bounce);

        sprite.setPosition(posicion);
    } else {
        sprite.setRotation(0.f);
        
        // Idle animation (Street Fighter style breathing)
        timerCaminar += deltaTime * 3.0f;
        float stretch = 1.0f + std::sin(timerCaminar) * 0.05f;
        float squash = 1.0f - std::sin(timerCaminar) * 0.05f;
        
        float actualScaleX = (sprite.getScale().x < 0) ? -escalaBase : escalaBase;
        sprite.setScale(actualScaleX * stretch, escalaBase * squash);
    }
}

void Jugador::limitarVentana() {
    // Limitar al tamaño del mapa
    if (posicion.x < 32.f) posicion.x = 32.f;
    if (posicion.x > ANCHO_MAPA_PIXELES - 32.f) posicion.x = ANCHO_MAPA_PIXELES - 32.f;
    if (posicion.y < 32.f) posicion.y = 32.f;
    if (posicion.y > ALTO_MAPA_PIXELES - 32.f) posicion.y = ALTO_MAPA_PIXELES - 32.f;

    sprite.setPosition(posicion);
}
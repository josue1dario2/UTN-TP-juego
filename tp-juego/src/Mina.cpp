#include "../include/Mina.h"

Mina::Mina(sf::Vector2f posicion){
    cargarTextura("assets/varios/mina.png");
    escalarSprite(5,5);
    setearTamanioSprite(10,6);
    centrarOrigen();
    setHitbox(120,120);

    mostrarHitbox = true;
    danio = 200;
    activa = false;
    temporizador = 0;
    cantidadFrames = 2;
    animacionActual = 0;
    velocidadAnimacion = 0.5f;
    destruir = false;
    exploto = false;
    
    
    setPosicionCentrado(posicion.x, posicion.y);
}

void Mina::actualizar(float deltaTime, const std::vector<sf::FloatRect> &listaZombies){
    temporizador += deltaTime;
    tiempoAnimacion += deltaTime;

    if (!destruir){
        if (temporizador > 2 && !activa){
            temporizador = 0;
            animacionActual = 1;
            tiempoAnimacion = 0;
        }
        
        if (tiempoAnimacion > velocidadAnimacion && animacionActual == 1){
            siguienteSprite();
            tiempoAnimacion = 0;
        }
        
        for (const auto &hitboxZombie : listaZombies) {
            if (hitbox.intersects(hitboxZombie) && !activa) {
                velocidadAnimacion = 0.1f;
                temporizador = 0;
                activa = true;
            }
        }
        
        if (activa && temporizador > 1) {
            std::cout << "explosion" << std::endl;
            setHitbox(250,250);
            setPosicionCentrado(getPosicion().x, getPosicion().y);
            exploto = true;
        }
    }
}

void Mina::explotar(){
    destruir = true;
}
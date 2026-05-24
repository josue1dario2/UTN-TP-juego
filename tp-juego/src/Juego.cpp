#include "../include/Juego.h"


Juego::Juego() : ventana(sf::VideoMode(800,600), "Mi Juego") {
    deltaTime = 0.f;
    ventana.setFramerateLimit(60);
}

// Ejecuta el bucle principal del juego
void Juego::iniciar(){
    while (ventana.isOpen()){

        //obtiene cuánto tiempo pasó desde el frame anterior y reinicia el reloj
        deltaTime = relojDelta.restart().asSeconds();

        procesarEventos();
        actualizar();
        renderizar();
    }

}

// Maneja eventos de ventana e input del usuario
void Juego::procesarEventos() {
    sf::Event evento;
    while (ventana.pollEvent(evento)){
        if(evento.type == sf::Event::Closed){
            ventana.close();
        }
    }
}

// Actualiza la logica del juego
void Juego::actualizar() {
    jugador.controlar();
}

// Dibuja todos los elementos en pantalla
void Juego::renderizar() {
    ventana.clear();

    // aca se dibujan las cosas

    jugador.dibujar(ventana);
    // ventana.draw(a);

    ventana.display();
}

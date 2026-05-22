#include "../include/Juego.h"


Juego::Juego() : ventana(sf::VideoMode(800,600), "Mi Juego") {
}

// Ejecuta el bucle principal del juego
void Juego::iniciar(){
    while (ventana.isOpen()){
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

}

// Dibuja todos los elementos en pantalla
void Juego::renderizar() {
    ventana.clear();

    // aca se dibujan las cosas

    ventana.display();
}

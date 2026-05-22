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


        
        sf::RectangleShape rect;
        sf::Vector2f posicion(400,300);

        rect.setPosition(posicion);
        rect.setSize(sf::Vector2f(100,100));






        procesarEventos();
        actualizar();
        renderizar(rect);
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
void Juego::renderizar(sf::RectangleShape rect) {
    ventana.clear();

    // aca se dibujan las cosas

    ventana.draw(rect);

    ventana.display();
}

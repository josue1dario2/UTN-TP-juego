#include "../include/Juego.h"


Juego::Juego() {
    deltaTime = 0.f;

    sf::VideoMode modoEscritorio = sf::VideoMode::getDesktopMode();

    ventana.create(modoEscritorio, "Mi Juego", sf::Style::Fullscreen);

    ventana.setFramerateLimit(60);

    vista.setSize(1280.f, 720.f);
    vista.setCenter(640.f, 360.f);
    ventana.setView(vista);


    // Inicialización de elementos del terreno
    obstaculos.emplace_back();
    obstaculos.back().cargarTextura("assets/casa.png");
    obstaculos.back().centrarOrigen();
    obstaculos.back().escalarSprite(3.f, 3.f);
    obstaculos.back().setHitbox(64.f * 3.f, 54.f * 3.f);
    obstaculos.back().setPosicion(1920.f, 1080.f);


}

// Ejecuta el bucle principal del juego
void Juego::iniciar(){

    texturaMapa.loadFromFile("assets/mapa.png");
    spriteMapa.setTexture(texturaMapa);

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
        if(evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape){
            ventana.close();
        }
    }
}

// Actualiza la logica del juego
void Juego::actualizar() {
    jugador.actualizar(deltaTime);

    //movimiento horizontal jugador, chequeo de colisiones mediante bucle for
    jugador.guardarPosicionAnterior();
    jugador.mover(jugador.getMovimientoX(), 0.f);
    for(auto& obstaculo : obstaculos) {
        if (jugador.getHitbox().intersects(obstaculo.getHitbox())) {
            jugador.volverPosicionAnteriorX();
        }
    }

    //movimiento vertical jugador
    jugador.guardarPosicionAnterior();
    jugador.mover(0.f, jugador.getMovimientoY());
    for(auto& obstaculo : obstaculos) {
        if (jugador.getHitbox().intersects(obstaculo.getHitbox())) {
            jugador.volverPosicionAnteriorY();
        }
    }


    // Seguir al jugador con la cámara
    vista.setCenter(jugador.getPosicion());
    ventana.setView(vista);
}

// Dibuja todos los elementos en pantalla
void Juego::renderizar() {
    ventana.clear();

    // aca se dibujan las cosas
    ventana.draw(spriteMapa);

    // Dibuja los obstáculos con un bucle
    for(auto& obstaculo : obstaculos) {
        obstaculo.dibujar(ventana);
    }

    jugador.dibujar(ventana);

    ventana.display();
}

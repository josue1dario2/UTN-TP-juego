#include "../include/Juego.h"


Juego::Juego() {
    deltaTime = 0.f;

    sf::VideoMode modoEscritorio = sf::VideoMode::getDesktopMode();

    ventana.create(modoEscritorio, "Mi Juego", sf::Style::Fullscreen);

    ventana.setFramerateLimit(60);

    vista.setSize(1280.f, 720.f);
    vista.setCenter(640.f, 360.f);
    ventana.setView(vista);
    


}

// Ejecuta el bucle principal del juego
void Juego::iniciar(){

    texturaMapa.loadFromFile("assets/mapa.png");
    spriteMapa.setTexture(texturaMapa);

    // Cargar la imagen en memoria para el sistema de colisiones por color
    imagenMapaColision.loadFromFile("assets/mapa.png");
    jugador.setMapaColision(&imagenMapaColision);

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


    sf::Vector2f posJugador = jugador.getPosicion();
    sf::Vector2u tamanoMapa = texturaMapa.getSize();
    sf::Vector2f tamanoVista = vista.getSize();

    float cx = posJugador.x;
    float cy = posJugador.y;

    // Limitar el centro de la cámara para que nunca muestre el exterior (el vacío negro)
    if (cx < tamanoVista.x / 2.f) cx = tamanoVista.x / 2.f;
    if (cx > tamanoMapa.x - tamanoVista.x / 2.f) cx = tamanoMapa.x - tamanoVista.x / 2.f;

    if (cy < tamanoVista.y / 2.f) cy = tamanoVista.y / 2.f;
    if (cy > tamanoMapa.y - tamanoVista.y / 2.f) cy = tamanoMapa.y - tamanoVista.y / 2.f;

    vista.setCenter(cx, cy);
    ventana.setView(vista);
}

// Dibuja todos los elementos en pantalla
void Juego::renderizar() {
    ventana.clear();

    // aca se dibujan las cosas

    ventana.draw(spriteMapa);
    jugador.dibujar(ventana);

    ventana.display();
}

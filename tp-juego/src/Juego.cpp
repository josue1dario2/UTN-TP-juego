#include "../include/Juego.h"


Juego::Juego() {
    deltaTime = 0.f;

    sf::VideoMode modoEscritorio = sf::VideoMode::getDesktopMode();

    ventana.create(modoEscritorio, "Mi Juego", sf::Style::Fullscreen);

    ventana.setMouseCursorVisible(false); // Ocultar el cursor estándar de la computadora

    ventana.setFramerateLimit(60);

    vista.setSize(1280.f, 720.f);
    vista.setCenter(640.f, 360.f);
    ventana.setView(vista);


    // Inicialización de elementos del terreno
    inicializarObstaculos(obstaculos);

}


void Juego::inicializarObstaculos(std::vector<ObjetoMapa>& obstaculos) {
    obstaculos.reserve(40);
    //casucha
    obstaculos.emplace_back();
    obstaculos.back().cargarTextura("assets/casa.png");
    obstaculos.back().centrarOrigen();
    obstaculos.back().escalarSprite(3.f, 3.f);
    obstaculos.back().setHitbox(64.f * 3.f, 54.f * 3.f);
    obstaculos.back().setPosicionCentrado(1920.f, 1080.f);
    
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(1058.f, 258.f);
    obstaculos.back().setPosicion(640.f, 540.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(1058.f, 258.f);
    obstaculos.back().setPosicion(640.f, 1300.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(1058.f, 258.f);
    obstaculos.back().setPosicion(2170.f, 1300.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(1058.f, 258.f);
    obstaculos.back().setPosicion(2170.f, 540.f);

    obstaculos.emplace_back();
    obstaculos.back().setHitbox(1058.f, 146.f);
    obstaculos.back().setPosicion(2170.f, 2014.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(1058.f, 146.f);
    obstaculos.back().setPosicion(636.f, 2014.f);

    obstaculos.emplace_back();
    obstaculos.back().setHitbox(1058.f, 72.f);
    obstaculos.back().setPosicion(2170.f, 0.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(1058.f, 72.f);
    obstaculos.back().setPosicion(640.f, 0.f);

    obstaculos.emplace_back();
    obstaculos.back().setHitbox(168.f, 72.f);
    obstaculos.back().setPosicion(0.f, 0.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(168.f, 258.f);
    obstaculos.back().setPosicion(0.f, 540.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(168.f, 258.f);
    obstaculos.back().setPosicion(0.f, 1300.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(168.f, 146.f);
    obstaculos.back().setPosicion(0.f, 2014.f);

    obstaculos.emplace_back();
    obstaculos.back().setHitbox(168.f, 72.f);
    obstaculos.back().setPosicion(3672.f, 0.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(168.f, 258.f);
    obstaculos.back().setPosicion(3672.f, 540.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(168.f, 258.f);
    obstaculos.back().setPosicion(3672.f, 1300.f);
    obstaculos.emplace_back();
    obstaculos.back().setHitbox(168.f, 146.f);
    obstaculos.back().setPosicion(3672.f, 2014.f);
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

        // Detectar clic izquierdo del mouse para disparar
        if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i posicionMouseVentana = sf::Mouse::getPosition(ventana);
            sf::Vector2f posicionMouseMundo = ventana.mapPixelToCoords(posicionMouseVentana);
            
            // Disparar el arma equipada apuntando al mouse
            jugador.getArma().disparar(jugador.getPosicion(), posicionMouseMundo);
        }
    }
}

// Actualiza la logica del juego
void Juego::actualizar() {
    jugador.actualizar(deltaTime);

    float deltaX = 12.f;
    float deltaY = 18.f;

    //movimiento horizontal jugador, chequeo de colisiones mediante bucle for
    jugador.guardarPosicionAnterior();
    jugador.mover(jugador.getMovimientoX(), 0.f);
    for(auto& obstaculo : obstaculos) {
        if (jugador.getHitbox().intersects(obstaculo.getHitbox())) {
            jugador.volverPosicionAnteriorX();
            break;
        }
    }

    //movimiento vertical jugador
    jugador.guardarPosicionAnterior();
    jugador.mover(0.f, jugador.getMovimientoY());
    for(auto& obstaculo : obstaculos) {
        if (jugador.getHitbox().intersects(obstaculo.getHitbox())) {
            jugador.volverPosicionAnteriorY();
            break;
        }
    }

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

    // Actualizar la mira personalizada y hacerla girar
    mira.actualizar(ventana, deltaTime);
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

    // Dibujar la estela del disparo del arma
    jugador.getArma().dibujar(ventana);

    // Dibujar el puntero personalizado (la mira giratoria) encima de todo
    mira.dibujar(ventana);

    ventana.display();
}
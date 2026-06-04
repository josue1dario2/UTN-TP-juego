#include "../include/Juego.h"



/*
    Juego::Juego(TipoPersonaje personajeElegido) {
    
    deltaTime = 0.f;

    jugador.seleccionarPersonaje(static_cast<int>(personajeElegido));

    sf::VideoMode modoEscritorio = sf::VideoMode::getDesktopMode();

    ventana.create(modoEscritorio, "Mi Juego", sf::Style::Fullscreen);

    ventana.setMouseCursorVisible(false); // Ocultar el cursor estándar de la computadora

    ventana.setFramerateLimit(60);

    vista.setSize(1280.f, 720.f);
    vista.setCenter(640.f, 360.f);
    ventana.setView(vista);


    // Inicialización de elementos del terreno
    inicializarObstaculos(obstaculos);
    texturaProyectil.loadFromFile("assets/bala.png");

}
*/
Juego::Juego() {
        pausa = false;
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
        texturaProyectil.loadFromFile("assets/bala.png");

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
void Juego::iniciar() {

    texturaMapa.loadFromFile("assets/mapa.png");
    spriteMapa.setTexture(texturaMapa);

    while (ventana.isOpen()) {

        deltaTime = relojDelta.restart().asSeconds();

        procesarEventos();

        if (pausa) {
            OpcionPausa opcion = menuPausa.mostrarMenuPausa(ventana);

            if (opcion == OpcionPausa::Seguir) {
                pausa = false;
            }
            else if (opcion == OpcionPausa::Salir) {
                ventana.close();
            }
        }
        else {
            actualizar();
        }

        renderizar();
    }
}

// Maneja eventos de ventana e input del usuario
void Juego::procesarEventos() {
    sf::Event evento;

    while (ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            ventana.close();
        }

        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
            pausa = true;
        }
    }
}


// Actualiza la logica del juego
void Juego::actualizar() {
    jugador.actualizar(deltaTime, obstaculos);
    jugador.getArma().actualizar(deltaTime, mira.getPosicion(), jugador.getPosicion(), proyectiles,texturaProyectil);

    for(auto& proyectil : proyectiles) {
        proyectil.actualizar(deltaTime, obstaculos);
    }

    proyectiles.erase(std::remove_if(proyectiles.begin(), proyectiles.end(),[](const Proyectil& p) {
        return p.debeDestruirse();
    }
    ), proyectiles.end()
    );

    auxVistaX = jugador.getPosicion().x;
    auxVistaY = jugador.getPosicion().y;

    // Limitar el centro de la cámara para que nunca muestre el exterior (el vacío negro)
    if (auxVistaX < vista.getSize().x / 2.f) auxVistaX = vista.getSize().x / 2.f;
    if (auxVistaX > texturaMapa.getSize().x - vista.getSize().x / 2.f) auxVistaX = texturaMapa.getSize().x - vista.getSize().x / 2.f;

    if (auxVistaY < vista.getSize().y / 2.f) auxVistaY = vista.getSize().y / 2.f;
    if (auxVistaY > texturaMapa.getSize().y - vista.getSize().y / 2.f) auxVistaY = texturaMapa.getSize().y - vista.getSize().y / 2.f;

    vista.setCenter(auxVistaX, auxVistaY);
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
    for(auto& proyectil : proyectiles) {
        proyectil.dibujar(ventana);
    }

    jugador.dibujar(ventana);
    jugador.getArma().dibujar(ventana);

    // Dibujar el puntero personalizado (la mira giratoria) encima de todo
    mira.dibujar(ventana);

    ventana.display();
}
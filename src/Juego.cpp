#include "Juego.h"
#include "Jugador.h"
#include "Zombie.h"
#include "Arma.h"
#include "Bala.h"
#include <iostream>
#include <cmath>

Juego::Juego() : ventana(sf::VideoMode(1280, 720), "Z-Wave", sf::Style::Close | sf::Style::Titlebar), 
               estadoActual(GameState::MENU_PRINCIPAL), jugador(nullptr), tiempoSupervivencia(0.0f), 
               transicionOleada(false), timerTransicionOleada(0.0f), enfriamientoDanoZombie(0.0f),
               camara(0.f, 0.f) {
    ventana.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Cargar mapa
    if (!mapa.cargarDesdeArchivo("data/Mapa.dat")) {
        mapa.generarMapaAleatorio();
    }
    
    // Texturas de respaldo (fallback) por si los assets no cargan
    sf::Image img;
    img.create(32, 32, sf::Color::Blue);
    texturaRespaldoJugador.loadFromImage(img);
    
    img.create(32, 32, sf::Color::Green);
    texturaRespaldoZombie.loadFromImage(img);
    
    img.create(8, 8, sf::Color::Yellow);
    texturaRespaldoBala.loadFromImage(img);
    
    // Suelo de respaldo
    img.create(256, 256, sf::Color(40, 40, 45));
    texturaSuelo.loadFromImage(img);
    figuraSuelo.setTexture(&texturaSuelo);
    figuraSuelo.setSize(sf::Vector2f(ANCHO_MAPA_PIXELES, ALTO_MAPA_PIXELES));
}

Juego::~Juego() {
    limpiar();
}

void Juego::limpiar() {
    if (jugador) {
        delete jugador;
        jugador = nullptr;
    }
    for (auto* z : zombies) {
        delete z;
    }
    zombies.clear();
    for (auto* b : balas) {
        delete b;
    }
    balas.clear();
}

void Juego::ejecutar() {
    cargarRecursos();
    cargarDatosJuego();
    
    sf::Font& font = recursos.tieneFuente("game_font") ? recursos.getFuente("game_font") : fuenteRespaldo;
    interfazHUD.inicializar(font);
    menu.inicializar(font, listaPersonajes);
    
    reloj.restart();
    
    // Game Loop principal
    while (ventana.isOpen()) {
        procesarEventos();
        float deltaTime = reloj.restart().asSeconds();
        actualizar(deltaTime);
        renderizar();
    }
}

void Juego::cargarRecursos() {
    recursos.cargarTextura("player_1", "assets/textures/player_tank.png");
    recursos.cargarTextura("player_2", "assets/textures/player_scout.png");
    recursos.cargarTextura("player_3", "assets/textures/player_medic.png");
    recursos.cargarTextura("zombie", "assets/textures/zombie_normal.png");
    recursos.cargarTextura("bala", "assets/textures/bullets/bullet.png");
    
    recursos.cargarTextura("ground", "assets/textures/tilemap.png");
    recursos.cargarFuente("game_font", "assets/fonts/game_font.ttf");
    
    // Cargar sonidos
    recursos.cargarBufferSonido("disparo", "assets/sounds/disparo.wav");
    recursos.cargarBufferSonido("explosion", "assets/sounds/explosion.wav");
    recursos.cargarBufferSonido("zombie_hit", "assets/sounds/zombie_hit.wav");
    recursos.cargarBufferSonido("jugador_dano", "assets/sounds/jugador_dano.wav");
    recursos.cargarBufferSonido("pickup", "assets/sounds/pickup.wav");
    recursos.cargarBufferSonido("habilidad_lista", "assets/sounds/habilidad_lista.wav");
    recursos.cargarBufferSonido("habilidad_uso", "assets/sounds/habilidad_uso.wav");
    recursos.cargarBufferSonido("oleada_completa", "assets/sounds/oleada_completa.wav");
    recursos.cargarBufferSonido("game_over", "assets/sounds/game_over.wav");
    recursos.cargarBufferSonido("zombie_gemido", "assets/sounds/zombie_gemido.wav");
    recursos.cargarBufferSonido("menu_seleccion", "assets/sounds/menu_seleccion.wav");
    
    if (recursos.tieneTextura("ground")) {
        sf::Texture& texOrg = recursos.getTextura("ground");
        texOrg.setRepeated(true);
        figuraSuelo.setTexture(&texOrg);
        figuraSuelo.setTextureRect(sf::IntRect(0, 0, ANCHO_MAPA_PIXELES, ALTO_MAPA_PIXELES));
    }
}

void Juego::cargarDatosJuego() {
    listaPersonajes = GestorDeArchivos::cargarPersonajes("data/Personajes.dat");
    listaArmas = GestorDeArchivos::cargarArmas("data/Armas.dat");
    std::vector<Estadistica> scores = GestorDeArchivos::cargarEstadisticas("data/Estadisticas.dat");
}

void Juego::inicializarJugador(int indicePersonaje) {
    if (jugador) delete jugador;
    
    Personaje data;
    if (indicePersonaje >= 0 && indicePersonaje < listaPersonajes.size()) {
        data = listaPersonajes[indicePersonaje];
    } else {
        data = Personaje(1, "Fallback", 100, 0, 150, "None");
    }

    std::string texName = "player_" + std::to_string(data.getId());
    sf::Texture& tex = recursos.tieneTextura(texName) ? recursos.getTextura(texName) : texturaRespaldoJugador;
    
    jugador = new Jugador(data, tex);
    jugador->setMapa(&mapa);
    sf::Vector2f spawnJugador = mapa.getPosicionSpawnValida();
    jugador->setPosicion(spawnJugador);
    camara.x = spawnJugador.x - 640.f;
    camara.y = spawnJugador.y - 360.f;
    darArmasJugador();
}

void Juego::darArmasJugador() {
    if (!jugador) return;
    for (const auto& wData : listaArmas) {
        jugador->agregarArma(new Arma(wData));
    }
    if (!listaArmas.empty()) {
        jugador->cambiarArma(0);
    }
}

void Juego::procesarEventos() {
    sf::Event event;
    while (ventana.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            ventana.close();
        }
        
        if (estadoActual == GameState::MENU_PRINCIPAL || 
            estadoActual == GameState::SELECCION_PERSONAJE || 
            estadoActual == GameState::GAME_OVER || 
            estadoActual == GameState::HIGH_SCORES) {
            
            MenuResult resultado = menu.procesarEntrada(event, ventana);
            manejarResultadoMenu(resultado);
        }
        else if (estadoActual == GameState::JUGANDO) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    estadoActual = GameState::PAUSADO;
                }
                if (event.key.code == sf::Keyboard::R) {
                    if (jugador) jugador->recargar();
                }
                if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num5) {
                    int index = event.key.code - sf::Keyboard::Num1;
                    if (jugador) jugador->cambiarArma(index);
                }
                // Habilidad especial con Shift
                if (event.key.code == sf::Keyboard::LShift) {
                    if (jugador && jugador->puedeUsarHabilidad()) {
                        sf::View view(sf::FloatRect(camara.x, camara.y, 1280.f, 720.f));
                        sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);
                        sf::Vector2f mousePosF = ventana.mapPixelToCoords(mousePos, view);
                        sf::Vector2f direccion = mousePosF - jugador->getPosicion();
                        jugador->activarHabilidad(direccion);
                    }
                }
            }
        }
        else if (estadoActual == GameState::PAUSADO) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                estadoActual = GameState::JUGANDO;
            }
        }
    }
}

void Juego::manejarResultadoMenu(MenuResult resultado) {
    switch (resultado) {
        case MenuResult::PLAY:
            estadoActual = GameState::SELECCION_PERSONAJE;
            menu.setMenuState(MenuState::SELECCION_PERSONAJE);
            break;
        case MenuResult::HIGH_SCORES:
            estadoActual = GameState::HIGH_SCORES;
            menu.setMenuState(MenuState::HIGH_SCORES);
            break;
        case MenuResult::QUIT:
            ventana.close();
            break;
        case MenuResult::BACK:
            estadoActual = GameState::MENU_PRINCIPAL;
            menu.setMenuState(MenuState::MENU_PRINCIPAL);
            break;
        case MenuResult::CHARACTER_SELECTED:
            reiniciarJuego();
            inicializarJugador(menu.getPersonajeSeleccionado());
            gestorOleadas.iniciarOleada(1);
            estadoActual = GameState::JUGANDO;
            break;
        case MenuResult::RETRY:
            estadoActual = GameState::SELECCION_PERSONAJE;
            menu.setMenuState(MenuState::SELECCION_PERSONAJE);
            break;
        default:
            break;
    }
}

void Juego::reiniciarJuego() {
    limpiar();
    tiempoSupervivencia = 0.0f;
    transicionOleada = false;
    timerTransicionOleada = 0.0f;
}

void Juego::actualizar(float deltaTime) {
    if (estadoActual == GameState::JUGANDO) {
        actualizarJugando(deltaTime);
    }
}

void Juego::actualizarJugando(float deltaTime) {
    tiempoSupervivencia += deltaTime;
    
    // Jugador
    if (jugador) {
        sf::View view(sf::FloatRect(camara.x, camara.y, 1280.f, 720.f));
        sf::Vector2i mousePosI = sf::Mouse::getPosition(ventana);
        sf::Vector2f mousePosF = ventana.mapPixelToCoords(mousePosI, view);
        jugador->apuntar(mousePosF);
        jugador->update(deltaTime);
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Texture& bTex = recursos.tieneTextura("bala") ? recursos.getTextura("bala") : texturaRespaldoBala;
            Entidad* newBala = jugador->disparar(mousePosF, bTex);
            if (newBala) {
                balas.push_back(static_cast<Bala*>(newBala));
            }
        }
    }
    
    // Oleadas
    gestorOleadas.actualizar(deltaTime);
    if (!gestorOleadas.oleadaCompletada() && gestorOleadas.debeSpawnearZombie()) {
        spawnearZombie();
    }
    
    // Zombies
    for (auto* z : zombies) {
        if (jugador) z->setObjetivo(jugador->getPosicion());
        z->update(deltaTime);
    }
    
    // Balas
    for (auto* b : balas) {
        b->update(deltaTime);
    }
    
    verificarColisiones();
    limpiarEntidadesMuertas();
    
    // Check wave transition
    if (gestorOleadas.oleadaCompletada() && zombies.empty()) {
        if (!transicionOleada) {
            transicionOleada = true;
            timerTransicionOleada = 5.0f;
            
            // Recompensa por sobrevivir la oleada
            if (jugador) {
                int oleadaActual = gestorOleadas.getOleadaActual();
                jugador->aumentarSaludMaxima(10);
                jugador->aumentarArmaduraMaxima(5);
            }
        } else {
            timerTransicionOleada -= deltaTime;
            if (timerTransicionOleada <= 0) {
                transicionOleada = false;
                gestorOleadas.iniciarOleada(gestorOleadas.getOleadaActual() + 1);
            }
        }
    }
    
    // Check Game Over
    if (jugador && !jugador->estaVivo()) {
        estadoActual = GameState::GAME_OVER;
        menu.setMenuState(MenuState::GAME_OVER);
        Estadistica entry("Player", jugador->getIdPersonaje(), jugador->getPuntos(), static_cast<int>(tiempoSupervivencia));
        GestorDeArchivos::guardarEstadistica("data/Estadisticas.dat", entry);
        menu.establecerDatosGameOver(entry.getPuntuacion(), gestorOleadas.getOleadaActual(), entry.getTiempoJugado());
    }
    
    // HUD
    if (jugador) {
        Arma* currentWeapon = jugador->getArmaActual();
        std::string wName = currentWeapon ? currentWeapon->getNombre() : "None";
        int ammoInMag = currentWeapon ? currentWeapon->getBalasEnMagazine() : 0;
        int totalAmmo = currentWeapon ? currentWeapon->getBalasTotales() : 0;
        
        interfazHUD.update(jugador->getSalud(), jugador->getSaludMaxima(), jugador->getArmadura(), jugador->getArmaduraMaxima(),
                   gestorOleadas.getOleadaActual(), jugador->getPuntos(), jugador->getPuntos(),
                   ammoInMag, totalAmmo, wName, tiempoSupervivencia, deltaTime);
                   
        if (transicionOleada && timerTransicionOleada > 2.0f) {
            interfazHUD.showWaveAnnouncement(gestorOleadas.getOleadaActual() + 1);
        }
    }
}

void Juego::spawnearZombie() {
    sf::Vector2f posSpawn = mapa.getPosicionSpawnValida();
    
    TipoZombie tipo = gestorOleadas.getZombieTypeForWave();
    sf::Texture* tex = &texturaRespaldoZombie;
    if (recursos.tieneTextura("zombie")) tex = &recursos.getTextura("zombie");
    
    Zombie* nuevoZombie = new Zombie(*tex, posSpawn, tipo);
    nuevoZombie->setMapa(&mapa);
    zombies.push_back(nuevoZombie);
}

void Juego::verificarColisiones() {
    // Bala vs Zombie
    for (auto* b : balas) {
        if (!b->estaVivo()) continue;
        for (auto* z : zombies) {
            if (!z->estaVivo()) continue;
            
            if (b->getLimites().intersects(z->getLimites())) {
                z->recibirDano(b->getDano());
                b->recibirDano(1000);
                if (!z->estaVivo() && jugador) {
                    jugador->agregarPuntos(z->getDineroAlMorir());
                    gestorOleadas.zombieMuerto();
                }
                break;
            }
        }
    }
    
    // Zombie vs Player - daño por contacto directo (distancia)
    if (jugador && jugador->estaVivo() && enfriamientoDanoZombie <= 0) {
        float radioContacto = 35.f; // radio de contacto en pixels
        for (auto* z : zombies) {
            if (z->estaVivo() && z->puedeAtacar()) {
                sf::Vector2f diff = z->getPosicion() - jugador->getPosicion();
                float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (dist < radioContacto) {
                    jugador->recibirDano(z->getDano());
                    z->atacar();
                    enfriamientoDanoZombie = 0.5f;

                    sf::Vector2f pushDir = diff / dist;
                    z->setPosicion(z->getPosicion() + pushDir * 30.f);
                }
            }
        }
    } else if (enfriamientoDanoZombie > 0) {
        enfriamientoDanoZombie -= 1.0f / 60.0f;
    }
}

void Juego::limpiarEntidadesMuertas() {
    for (auto it = zombies.begin(); it != zombies.end();) {
        if (!(*it)->estaVivo()) {
            delete *it;
            it = zombies.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = balas.begin(); it != balas.end();) {
        if (!(*it)->estaVivo()) {
            delete *it;
            it = balas.erase(it);
        } else {
            ++it;
        }
    }
}

void Juego::guardarPartida() {
    if (!jugador) return;
    
    Partida partida;
    partida.setIdPartida(1);
    partida.setOleadaActual(gestorOleadas.getOleadaActual());
    partida.setPuntos(jugador->getPuntos());
    partida.setTiempoJugado(static_cast<int>(tiempoSupervivencia));
    partida.setIdPersonaje(jugador->getIdPersonaje());
    partida.setNombrePersonaje(jugador->getNombre());
    
    GestorDeArchivos::guardarPartida("data/PartidaActual.dat", partida);
}

Partida Juego::cargarPartida() {
    return GestorDeArchivos::cargarPartida("data/PartidaActual.dat");
}

void Juego::renderizar() {
    ventana.clear(sf::Color(20, 20, 25));
    
    if (estadoActual == GameState::JUGANDO || estadoActual == GameState::PAUSADO || estadoActual == GameState::GAME_OVER) {
        if (jugador) {
            sf::Vector2f posJugador = jugador->getPosicion();
            camara.x = posJugador.x - 640.f;
            camara.y = posJugador.y - 360.f;
            
            if (camara.x < 0) camara.x = 0;
            if (camara.y < 0) camara.y = 0;
            if (camara.x > ANCHO_MAPA_PIXELES - 1280) camara.x = ANCHO_MAPA_PIXELES - 1280;
            if (camara.y > ALTO_MAPA_PIXELES - 720) camara.y = ALTO_MAPA_PIXELES - 720;
        }
        
        sf::View view(sf::FloatRect(camara.x, camara.y, 1280.f, 720.f));
        ventana.setView(view);
        
        dibujarSuelo();
        mapa.render(ventana, camara);
        
        for (auto* z : zombies) {
            z->render(ventana);
        }
        for (auto* b : balas) {
            b->render(ventana);
        }
        if (jugador) {
            jugador->render(ventana);
        }
        
        ventana.setView(ventana.getDefaultView());
        
        interfazHUD.render(ventana);
        
        if (estadoActual == GameState::PAUSADO) {
            sf::Text pt;
            sf::Font& font = recursos.tieneFuente("game_font") ? recursos.getFuente("game_font") : fuenteRespaldo;
            pt.setFont(font);
            pt.setString("PAUSADO");
            pt.setCharacterSize(48);
            pt.setFillColor(sf::Color::White);
            pt.setPosition(1280/2 - pt.getGlobalBounds().width/2, 720/2 - pt.getGlobalBounds().height/2);
            ventana.draw(pt);
        }
    }
    
    if (estadoActual == GameState::MENU_PRINCIPAL || estadoActual == GameState::SELECCION_PERSONAJE || estadoActual == GameState::HIGH_SCORES || estadoActual == GameState::GAME_OVER) {
        MenuState mState = MenuState::MENU_PRINCIPAL;
        if (estadoActual == GameState::SELECCION_PERSONAJE) mState = MenuState::SELECCION_PERSONAJE;
        else if (estadoActual == GameState::HIGH_SCORES) mState = MenuState::HIGH_SCORES;
        else if (estadoActual == GameState::GAME_OVER) mState = MenuState::GAME_OVER;
        menu.dibujar(ventana, mState);
    }
    
    ventana.display();
}

void Juego::dibujarSuelo() {
    ventana.draw(figuraSuelo);
}
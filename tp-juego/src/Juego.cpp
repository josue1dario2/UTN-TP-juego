#include "../include/Juego.h"
#include "../include/SoundManager.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>

Juego::Juego() {
  // Inicialización de sonidos
  SoundManager::cargarSonidos();

  deltaTime = 0.f;
  indiceMenuSeleccionado = 0;

  sf::VideoMode modoEscritorio = sf::VideoMode::getDesktopMode();
  ventana.create(modoEscritorio, "The Last Squad", sf::Style::Fullscreen);
  ventana.setMouseCursorVisible(false); // Ocultar el cursor estándar para usar la mira personalizada
  ventana.setFramerateLimit(60);
  ventana.requestFocus(); // Forzar el foco de la ventana

  vista.setSize(1280.f, 720.f);
  vista.setCenter(640.f, 360.f);
  ventana.setView(vista);

  // Inicialización de elementos del terreno
  inicializarObstaculos(obstaculos);

  // Zonas de spawn de zombies
  std::vector<sf::FloatRect> zonasSpawn = {
      sf::FloatRect(200.f, 100.f, 400.f, 350.f),  // Superior Izquierda
      sf::FloatRect(3250.f, 100.f, 400.f, 350.f), // Superior Derecha
      sf::FloatRect(200.f, 850.f, 400.f, 350.f),  // Central Izquierda
      sf::FloatRect(3250.f, 850.f, 400.f, 350.f), // Central Derecha
      sf::FloatRect(200.f, 1600.f, 400.f, 350.f), // Inferior Izquierda
      sf::FloatRect(3250.f, 1600.f, 400.f, 350.f) // Inferior Derecha
  };
  zombieManager.inicializarZonasSpawn(zonasSpawn);

  texturaProyectil.loadFromFile("assets/armas/bala.png");
  hud.inicializar(0);

  trazaMosin.setFillColor(sf::Color::White);
  trazaMosin.setSize(sf::Vector2f(3000.f, 3.f));
  trazaMosin.setOrigin(0.f, 1.5f);

  // Sembrado por defecto de personajes si personajes.dat no tiene nada
  FILE* pFileTest = fopen("personajes.dat", "rb");
  if (!pFileTest) {
      archivoPersonaje archivo("personajes.dat");
      
      RegistroPersonaje recon;
      recon.id = 0;
      recon.idArmaEspecial = 4;
      std::strcpy(recon.nombre, "Recon");
      recon.vida = 100.f;
      recon.armadura = 50.f;
      recon.velocidad = 250.f;
      recon.cooldownHabilidad = 15.f;
      archivo.grabarRegistroPersonaje(recon);

      RegistroPersonaje joel;
      joel.id = 1;
      joel.idArmaEspecial = 4;
      std::strcpy(joel.nombre, "Joel");
      joel.vida = 150.f;
      joel.armadura = 100.f;
      joel.velocidad = 180.f;
      joel.cooldownHabilidad = 20.f;
      archivo.grabarRegistroPersonaje(joel);
  } else {
      fclose(pFileTest);
  }

  // Cargar estadísticas históricas
  cargarStats();

  // Cargar fondo del menú personalizado
  tieneFondoMenu = texturaFondoMenu.loadFromFile("assets/menu_bg.png");
  if (tieneFondoMenu) {
      spriteFondoMenu.setTexture(texturaFondoMenu);
      sf::Vector2u size = texturaFondoMenu.getSize();
      spriteFondoMenu.setScale(1280.f / size.x, 720.f / size.y);
  }

  inicializarMenus();
  estadoActual = EstadoJuego::MenuPrincipal;
}

void Juego::inicializarObstaculos(std::vector<ObjetoMapa> &obstaculos) {
  obstaculos.reserve(40);

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

void Juego::iniciar() {
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  texturaMapa.loadFromFile("assets/varios/mapa.png");
  spriteMapa.setTexture(texturaMapa);
  proyectiles.reserve(100);

  trampas.emplace_back(sf::Vector2f(1920,1080));
  while (ventana.isOpen()) {
    deltaTime = relojDelta.restart().asSeconds();
    procesarEventos();
    actualizar();
    renderizar();
  }
}

void Juego::inicializarMenus() {
  if (!fuenteMenu.loadFromFile("assets/minecraft.ttf")) {
      std::cerr << "Error: No se pudo cargar assets/minecraft.ttf para el menú" << std::endl;
  }
  
  tituloJuego.setFont(fuenteMenu);
  tituloJuego.setString("THE LAST SQUAD");
  tituloJuego.setCharacterSize(50);
  tituloJuego.setFillColor(sf::Color::Red);
  sf::FloatRect bounds = tituloJuego.getLocalBounds();
  tituloJuego.setOrigin(bounds.left + bounds.width/2.f, bounds.top + bounds.height/2.f);
  tituloJuego.setPosition(640.f, 120.f);
  
  // Botones menú principal reestructurado
  btnMenuJugar = Boton(515.f, 350.f, 250.f, 50.f, fuenteMenu, "JUGAR", 
                       sf::Color(25, 25, 25, 220), sf::Color(100, 20, 20), sf::Color(150, 30, 30));
  
  btnMenuStats = Boton(515.f, 430.f, 250.f, 50.f, fuenteMenu, "ESTADISTICAS", 
                       sf::Color(25, 25, 25, 220), sf::Color(100, 20, 20), sf::Color(150, 30, 30));
                          
  btnMenuSalir = Boton(515.f, 510.f, 250.f, 50.f, fuenteMenu, "SALIR", 
                       sf::Color(25, 25, 25, 220), sf::Color(100, 20, 20), sf::Color(150, 30, 30));

  // Volver de Selección y de Estadísticas
  btnVolverSeleccion = Boton(515.f, 600.f, 250.f, 50.f, fuenteMenu, "VOLVER", 
                             sf::Color(25, 25, 25, 220), sf::Color(80, 80, 80), sf::Color(120, 120, 120));

  btnVolverStats = Boton(515.f, 580.f, 250.f, 50.f, fuenteMenu, "VOLVER", 
                         sf::Color(25, 25, 25, 220), sf::Color(80, 80, 80), sf::Color(120, 120, 120));

  // Textos para Estadísticas
  textoStats.setFont(fuenteMenu);
  textoStats.setCharacterSize(22);
  textoStats.setFillColor(sf::Color::White);

  // Cargar personajes para botones de selección
  botonesPersonajes.clear();

  // Contar cuántos personajes hay en la base de datos
  int totalPersonajes = 0;
  FILE* pFileCount = fopen("personajes.dat", "rb");
  if (pFileCount) {
      RegistroPersonaje temp;
      while (fread(&temp, sizeof(temp), 1, pFileCount) == 1) {
          totalPersonajes++;
      }
      fclose(pFileCount);
  }

  FILE* pFile = fopen("personajes.dat", "rb");
  if (pFile) {
      RegistroPersonaje reg;
      int idx = 0;
      while (fread(&reg, sizeof(reg), 1, pFile) == 1) {
          float btnW = 320.f;
          float btnH = 140.f;
          float btnX = 0.f;
          float btnY = 0.f;

          if (totalPersonajes <= 3) {
              // Una sola fila centrada
              float startX = 640.f - (totalPersonajes * 350.f - 30.f) / 2.f;
              btnX = startX + idx * 350.f;
              btnY = 300.f;
              btnW = 320.f;
              btnH = 150.f;
          } else {
              // Dos filas centraditas
              int personajesFila1 = (totalPersonajes + 1) / 2;
              if (personajesFila1 > 3) personajesFila1 = 3;
              int personajesFila2 = totalPersonajes - personajesFila1;

              if (idx < personajesFila1) {
                  float startX = 640.f - (personajesFila1 * 350.f - 30.f) / 2.f;
                  btnX = startX + idx * 350.f;
                  btnY = 220.f;
              } else {
                  int idxFila2 = idx - personajesFila1;
                  float startX = 640.f - (personajesFila2 * 350.f - 30.f) / 2.f;
                  btnX = startX + idxFila2 * 350.f;
                  btnY = 390.f;
              }
              btnW = 320.f;
              btnH = 140.f;
          }

          std::string nombreAMostrar = reg.nombre;
          if (!nombreAMostrar.empty()) {
              nombreAMostrar[0] = std::toupper(nombreAMostrar[0]);
          }

          Boton btn(btnX, btnY, btnW, btnH, fuenteMenu, 
                    nombreAMostrar + "\nVida: " + std::to_string((int)reg.vida) + "\nEscudo: " + std::to_string((int)reg.armadura),
                    sf::Color(25, 25, 25, 220), sf::Color(80, 30, 30), sf::Color(120, 40, 40));
          
          // Posicionar el texto a la derecha del botón
          btn.setTextoPosicion(btnX + 215.f, btnY + btnH / 2.f);

          BotonPersonaje bp;
          bp.boton = btn;
          bp.registro = reg;

          std::string nombreLower = reg.nombre;
          std::transform(nombreLower.begin(), nombreLower.end(), nombreLower.begin(), ::tolower);
          if (nombreLower == "johnwick") {
              nombreLower = "johnWick";
          }
          std::string rutaTextura = "assets/personajes/" + nombreLower + ".png";

          if (bp.textura.loadFromFile(rutaTextura)) {
              bp.sprite.setTexture(bp.textura);
              sf::Vector2u texSize = bp.textura.getSize();
              int frameW = texSize.x;
              int frameH = texSize.y;
              bp.sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
              
              // Escala adaptativa para que quepa bien en la tarjeta
              float scale = 90.f / frameH;
              if (scale > 3.0f) scale = 3.0f;
              bp.sprite.setScale(scale, scale);
              
              bp.sprite.setOrigin(frameW / 2.f, frameH / 2.f);
              bp.sprite.setPosition(btnX + 70.f, btnY + btnH / 2.f); // A la izquierda del recuadro
          }

          botonesPersonajes.push_back(bp);
          idx++;
      }
      fclose(pFile);
      
      // Re-vincular las texturas a los sprites
      for (auto& bp : botonesPersonajes) {
          bp.sprite.setTexture(bp.textura);
      }
  }
}

void Juego::procesarEventos() {
  sf::Event evento;
  sf::Vector2f posMouse = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana), ventana.getDefaultView());

  while (ventana.pollEvent(evento)) {
    if (evento.type == sf::Event::Closed) {
      ventana.close();
    }
    
    // Controles específicos de estados
    if (estadoActual == EstadoJuego::MenuPrincipal) {
        if (evento.type == sf::Event::KeyPressed) {
            if (evento.key.code == sf::Keyboard::W || evento.key.code == sf::Keyboard::Up) {
                indiceMenuSeleccionado--;
                if (indiceMenuSeleccionado < 0) indiceMenuSeleccionado = 2;
            }
            else if (evento.key.code == sf::Keyboard::S || evento.key.code == sf::Keyboard::Down) {
                indiceMenuSeleccionado++;
                if (indiceMenuSeleccionado > 2) indiceMenuSeleccionado = 0;
            }
            else if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Space) {
                SoundManager::play("click_boton");
                if (indiceMenuSeleccionado == 0) {
                    estadoActual = EstadoJuego::SeleccionPersonaje;
                }
                else if (indiceMenuSeleccionado == 1) {
                    estadoActual = EstadoJuego::Estadisticas;
                }
                else if (indiceMenuSeleccionado == 2) {
                    ventana.close();
                }
            }
        }
        else if (evento.type == sf::Event::MouseButtonPressed) {
            if (btnMenuJugar.fueClickeado(posMouse, evento.mouseButton.button)) {
                SoundManager::play("click_boton");
                estadoActual = EstadoJuego::SeleccionPersonaje;
            }
            else if (btnMenuStats.fueClickeado(posMouse, evento.mouseButton.button)) {
                SoundManager::play("click_boton");
                estadoActual = EstadoJuego::Estadisticas;
            }
            else if (btnMenuSalir.fueClickeado(posMouse, evento.mouseButton.button)) {
                SoundManager::play("click_boton");
                ventana.close();
            }
        }
    }
    else if (estadoActual == EstadoJuego::Estadisticas) {
        if (evento.type == sf::Event::MouseButtonPressed) {
            if (btnVolverStats.fueClickeado(posMouse, evento.mouseButton.button)) {
                SoundManager::play("click_boton");
                estadoActual = EstadoJuego::MenuPrincipal;
            }
        }
    }
    else if (estadoActual == EstadoJuego::SeleccionPersonaje) {
        if (evento.type == sf::Event::MouseButtonPressed) {
            for (auto& bp : botonesPersonajes) {
                if (bp.boton.fueClickeado(posMouse, evento.mouseButton.button)) {
                    SoundManager::play("click_boton");
                    personajeSeleccionado = bp.registro;
                    float velocidad = personajeSeleccionado.velocidad;
                    float cooldown = personajeSeleccionado.cooldownHabilidad;
                    jugador = Personaje(personajeSeleccionado.id, personajeSeleccionado.idArmaEspecial, 
                                        personajeSeleccionado.nombre, personajeSeleccionado.vida, personajeSeleccionado.armadura, 
                                        velocidad, cooldown);
                    hud.inicializar(personajeSeleccionado.id);
                    zombieManager.cargarOleada(1);
                    zombieManager.resetZombiesEliminados();
                    estadoActual = EstadoJuego::Jugando;
                    ventana.setMouseCursorVisible(false);
                    break;
                }
            }
            if (btnVolverSeleccion.fueClickeado(posMouse, evento.mouseButton.button)) {
                SoundManager::play("click_boton");
                estadoActual = EstadoJuego::MenuPrincipal;
            }
        }
    }
    else if (estadoActual == EstadoJuego::Jugando) {
        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
            // Registrar estadísticas al abandonar
            statsHistoricas.registrarNuevaPartida();
            statsHistoricas.registrarOleadaMaxima(zombieManager.getOleadaActual());
            statsHistoricas.sumarZombiesEliminados(zombieManager.getZombiesEliminados());
            guardarStats();

            estadoActual = EstadoJuego::MenuPrincipal;
            ventana.setMouseCursorVisible(false);
        }
    }
  }
}

void Juego::actualizar() {
  sf::Vector2f posMouse = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana), ventana.getDefaultView());
  

  // Actualizar siempre la mira para usarla como puntero con la vista correspondiente
  if (estadoActual == EstadoJuego::Jugando) {
      mira.actualizar(ventana, vista, deltaTime);
      ventana.setMouseCursorVisible(false);
  } else {
      mira.actualizar(ventana, ventana.getDefaultView(), deltaTime);
      ventana.setMouseCursorVisible(false);
  }

  if (estadoActual == EstadoJuego::MenuPrincipal) {
      actualizarMenu(posMouse);
  }
  else if (estadoActual == EstadoJuego::Estadisticas) {
      actualizarEstadisticas(posMouse);
  }
  else if (estadoActual == EstadoJuego::SeleccionPersonaje) {
      actualizarSeleccionPersonaje(posMouse);
  }
  else if (estadoActual == EstadoJuego::Jugando) {
      std::vector<sf::FloatRect> hitboxesZombies = zombieManager.getHitboxesZombies();

      if (jugador.estaVivo()) {
        jugador.actualizar(deltaTime, obstaculos, hitboxesZombies, mira.getPosicion(), trampas, sf::Vector2f(texturaMapa.getSize().x, texturaMapa.getSize().y));
        
        // Lógica de compra automática de recarga si no le queda reserva al presionar 'R'
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !jugador.getArma().getEnRecarga()) {
            Arma& armaActiva = jugador.getArma();
            int idArma = armaActiva.getIdArma();
            // Evitamos armas infinitas: Cuchillo (0), Arco (5), Katana (7)
            if (idArma != 0 && idArma != 5 && idArma != 7) {
                if (armaActiva.getMunicionActual() == 0 && armaActiva.getMunicionEnCargador() < armaActiva.getTamanioCargador()) {
                    if (jugador.getDinero() >= 350) {
                        jugador.sumarDinero(-350);
                        armaActiva.comprarMunicion(armaActiva.getTamanioCargador());
                    }
                }
            }
        }

        jugador.getArma().actualizar(deltaTime, mira.getPosicion(), jugador.getPosicion(), proyectiles, texturaProyectil);
        vista.setSize(1280.f * jugador.getMultiplicadorZoom(), 720.f * jugador.getMultiplicadorZoom());
      } else {
        // Registrar estadísticas de derrota
        statsHistoricas.registrarNuevaPartida();
        statsHistoricas.registrarOleadaMaxima(zombieManager.getOleadaActual());
        statsHistoricas.sumarZombiesEliminados(zombieManager.getZombiesEliminados());
        guardarStats();

        estadoActual = EstadoJuego::GameOver;
        ventana.setMouseCursorVisible(false);
      }
      
      procesarRayCast();

      for (auto &proyectil : proyectiles) {
        proyectil.actualizar(deltaTime, obstaculos);
      }
      proyectiles.erase(std::remove_if(proyectiles.begin(), proyectiles.end(), [](const Proyectil &p) { return p.debeDestruirse(); }), proyectiles.end());

      // -------- trampas (minas) --------
      for (auto &trampa : trampas) {
        trampa.actualizar(deltaTime, zombieManager.getHitboxesZombies());
      }
      trampas.erase(std::remove_if(trampas.begin(), trampas.end(), [](const Mina &m) { return m.debeDestruirse(); }), trampas.end());

      // -------- Lógica de zombies y colisión de balas delegada en ZombieManager --------
      zombieManager.actualizar(deltaTime, jugador, obstaculos, proyectiles, trampas);

      auxVistaX = jugador.getPosicion().x;
      auxVistaY = jugador.getPosicion().y;

      if (auxVistaX < vista.getSize().x / 2.f)
        auxVistaX = vista.getSize().x / 2.f;
      if (auxVistaX > texturaMapa.getSize().x - vista.getSize().x / 2.f)
        auxVistaX = texturaMapa.getSize().x - vista.getSize().x / 2.f;

      if (auxVistaY < vista.getSize().y / 2.f)
        auxVistaY = vista.getSize().y / 2.f;
      if (auxVistaY > texturaMapa.getSize().y - vista.getSize().y / 2.f)
        auxVistaY = texturaMapa.getSize().y - vista.getSize().y / 2.f;

      vista.setCenter(auxVistaX, auxVistaY);
      ventana.setView(vista);

      hud.actualizar(jugador, zombieManager);
  }
  else if (estadoActual == EstadoJuego::GameOver) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
          estadoActual = EstadoJuego::MenuPrincipal;
      }
  }
}

void Juego::renderizar() {
  ventana.clear();

  if (estadoActual == EstadoJuego::MenuPrincipal) {
      renderizarMenu();
  }
  else if (estadoActual == EstadoJuego::Estadisticas) {
      renderizarEstadisticas();
  }
  else if (estadoActual == EstadoJuego::SeleccionPersonaje) {
      renderizarSeleccionPersonaje();
  }
  else if (estadoActual == EstadoJuego::Jugando) {
      ventana.setView(vista);
      ventana.draw(spriteMapa);

      for (auto &trampa : trampas) {
        trampa.dibujar(ventana);
      }

      for (auto &obstaculo : obstaculos) {
        obstaculo.dibujar(ventana);
      }

      for (auto &proyectil : proyectiles) {
        proyectil.dibujar(ventana);
      }

      if(mostrarTrazaMosin) {
        ventana.draw(trazaMosin);
      }
      
      if (jugador.estaVivo()) {
        jugador.dibujar(ventana);
        jugador.getArma().dibujar(ventana);
      }

      zombieManager.dibujarZombies(ventana);
      mira.dibujar(ventana);
      
      ventana.setView(ventana.getDefaultView());
      hud.dibujar(ventana);
  }
  else if (estadoActual == EstadoJuego::GameOver) {
      if (tieneFondoMenu) {
          ventana.setView(ventana.getDefaultView());
          ventana.draw(spriteFondoMenu);
      }
      
      // Panel oscuro translúcido
      sf::RectangleShape panel(sf::Vector2f(800.f, 250.f));
      panel.setFillColor(sf::Color(0, 0, 0, 220));
      panel.setOutlineColor(sf::Color::Red);
      panel.setOutlineThickness(2.f);
      panel.setPosition(240.f, 235.f);
      ventana.draw(panel);

      sf::Text txtGameOver;
      txtGameOver.setFont(fuenteMenu);
      txtGameOver.setString("GAME OVER\n\nPresione ESPACIO o ENTER\npara volver al menu");
      txtGameOver.setCharacterSize(26);
      txtGameOver.setFillColor(sf::Color::Red);
      sf::FloatRect bounds = txtGameOver.getLocalBounds();
      txtGameOver.setOrigin(bounds.left + bounds.width/2.f, bounds.top + bounds.height/2.f);
      txtGameOver.setPosition(640.f, 360.f);
      ventana.draw(txtGameOver);
      
      // Dibujar mira sobre la pantalla de GameOver
      mira.dibujar(ventana);
  }

  ventana.display();
}

void Juego::actualizarMenu(sf::Vector2f posMouse) {
    if (btnMenuJugar.getGlobalBounds().contains(posMouse)) indiceMenuSeleccionado = 0;
    else if (btnMenuStats.getGlobalBounds().contains(posMouse)) indiceMenuSeleccionado = 1;
    else if (btnMenuSalir.getGlobalBounds().contains(posMouse)) indiceMenuSeleccionado = 2;

    btnMenuJugar.actualizar(posMouse, indiceMenuSeleccionado == 0);
    btnMenuStats.actualizar(posMouse, indiceMenuSeleccionado == 1);
    btnMenuSalir.actualizar(posMouse, indiceMenuSeleccionado == 2);
}

void Juego::renderizarMenu() {
    ventana.setView(ventana.getDefaultView());
    if (tieneFondoMenu) {
        ventana.draw(spriteFondoMenu);
    } else {
        ventana.draw(tituloJuego);
    }
    btnMenuJugar.dibujar(ventana);
    btnMenuStats.dibujar(ventana);
    btnMenuSalir.dibujar(ventana);
    
    // Dibujar mira personalizada sobre los botones
    mira.dibujar(ventana);
}

void Juego::actualizarEstadisticas(sf::Vector2f posMouse) {
    btnVolverStats.actualizar(posMouse);
}

void Juego::renderizarEstadisticas() {
    ventana.setView(ventana.getDefaultView());
    if (tieneFondoMenu) {
        ventana.draw(spriteFondoMenu);
    }
    
    std::string statsStr = "ESTADISTICAS HISTORICAS\n\n"
                           "Partidas jugadas: " + std::to_string(statsHistoricas.getPartidasJugadas()) + "\n"
                           "Oleada maxima alcanzada: " + std::to_string(statsHistoricas.getOleadaMaxima()) + "\n"
                           "Zombies eliminados: " + std::to_string(statsHistoricas.getZombiesEliminados());
    
    textoStats.setString(statsStr);
    sf::FloatRect bounds = textoStats.getLocalBounds();
    textoStats.setOrigin(bounds.left + bounds.width/2.f, bounds.top + bounds.height/2.f);
    textoStats.setPosition(640.f, 300.f);
    
    ventana.draw(textoStats);
    btnVolverStats.dibujar(ventana);
    mira.dibujar(ventana);
}

void Juego::actualizarSeleccionPersonaje(sf::Vector2f posMouse) {
    for (auto& bp : botonesPersonajes) {
        bp.boton.actualizar(posMouse);
    }
    btnVolverSeleccion.actualizar(posMouse);
}

void Juego::renderizarSeleccionPersonaje() {
    ventana.setView(ventana.getDefaultView());
    if (tieneFondoMenu) {
        ventana.draw(spriteFondoMenu);
    }

    sf::Text txtSelect;
    txtSelect.setFont(fuenteMenu);
    txtSelect.setString("SELECCIONA TU PERSONAJE:");
    txtSelect.setCharacterSize(35);
    txtSelect.setFillColor(sf::Color::White);
    sf::FloatRect bounds = txtSelect.getLocalBounds();
    txtSelect.setOrigin(bounds.left + bounds.width/2.f, bounds.top + bounds.height/2.f);
    txtSelect.setPosition(640.f, 150.f);
    
    ventana.draw(txtSelect);
    for (auto& bp : botonesPersonajes) {
        bp.boton.dibujar(ventana);
        ventana.draw(bp.sprite);
    }
    btnVolverSeleccion.dibujar(ventana);
    
    // Dibujar mira sobre la selección
    mira.dibujar(ventana);
}

void Juego::iniciarPartidaDirecta() {
    RegistroPersonaje reg;
    bool cargado = false;
    FILE* pFile = fopen("personajes.dat", "rb");
    if (pFile) {
        if (fread(&reg, sizeof(reg), 1, pFile) == 1) {
            cargado = true;
        }
        fclose(pFile);
    }
    
    if (cargado) {
        float velocidad = reg.velocidad;
        float cooldown = reg.cooldownHabilidad;
        jugador = Personaje(reg.id, reg.idArmaEspecial, 
                            reg.nombre, reg.vida, reg.armadura, 
                            velocidad, cooldown);
    } else {
        // Fallback default
        jugador = Personaje(0, 4, "Jugador", 100.f, 50.f, 250.f, 15.f);
    }
    zombieManager.cargarOleada(1);
    zombieManager.resetZombiesEliminados();
    estadoActual = EstadoJuego::Jugando;
    ventana.setMouseCursorVisible(false);
}

void Juego::procesarRayCast(){
  if(mostrarTrazaMosin) {
    tiempoTrazaMosin -= deltaTime;
    if(tiempoTrazaMosin <= 0.f) {
      mostrarTrazaMosin = false;
    }
  }
  
  if(jugador.getArma().spawnRayCast) {
    sf::Vector2f origen = jugador.getPosicion();
    sf::Vector2f direccion;
    direccion.x = mira.getPosicion().x - origen.x;
    direccion.y = mira.getPosicion().y - origen.y;
    
    float longitud = std::sqrt(direccion.x*direccion.x + direccion.y * direccion.y);
    
    direccion.x /= longitud;
    direccion.y /= longitud;

    float angulo = std::atan2(direccion.y, direccion.x)* 180.f / 3.14159f;
    trazaMosin.setPosition(origen.x,origen.y+10);
    trazaMosin.setRotation(angulo);

    mostrarTrazaMosin = true;
    tiempoTrazaMosin = 0.05f;
    float distanciaImpacto;
    float alcance = jugador.getArma().getAlcance();
    
    for (float distancia = 0.f; distancia < alcance; distancia += 5.f) {
      sf::Vector2f punto;
      
      punto.x = origen.x + direccion.x * distancia;
      punto.y = origen.y + direccion.y * distancia;
      trazaMosin.setSize(sf::Vector2f(alcance, 3.f));
      for(auto& obstaculo : obstaculos) {
        if (obstaculo.getHitbox().contains(punto)) {
          distanciaImpacto = distancia;
          trazaMosin.setSize(sf::Vector2f(distanciaImpacto, 3.f));
          return;
        }
      }
      for(auto& zombie : zombieManager.getZombies()) {
        if(zombie.getHitbox().contains(punto)) {
          zombie.recibirDanio(jugador.getArma().getDanio());
        }
      }
    }
  }
}

void Juego::guardarStats() {
    std::ofstream archivo("stats.dat", std::ios::binary | std::ios::trunc);
    if (archivo) {
        archivo.write(reinterpret_cast<char*>(&statsHistoricas), sizeof(Estadistica));
        archivo.close();
    }
}

void Juego::cargarStats() {
    std::ifstream archivo("stats.dat", std::ios::binary);
    if (archivo) {
        archivo.read(reinterpret_cast<char*>(&statsHistoricas), sizeof(Estadistica));
        archivo.close();
    } else {
        statsHistoricas.setPartidasJugadas(0);
        statsHistoricas.setOleadaMaxima(0);
        statsHistoricas.setZombiesEliminados(0);
    }
}
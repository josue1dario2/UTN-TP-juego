#pragma once
#include <string>
#include <iostream>
#include "../include/Menu.h"

Menu::Menu() {
    sf::VideoMode modo = sf::VideoMode::getDesktopMode();

    ventana.create(modo, "Menu Principal", sf::Style::Fullscreen);

    ventana.setFramerateLimit(60);

    vista.setSize(1280.f, 720.f);
    vista.setCenter(640.f, 360.f);

    ventana.setView(vista);

    if (!fuente.loadFromFile("assets/fonts/game_font.ttf")) {
        std::cout << "Error cargando fuente" << std::endl;
    }

    opcionActual = 0;
    personajeActual = 0;
}

void Menu::dibujarBoton(sf::RectangleShape& boton, float x, float y, bool seleccionado) {
    boton.setSize(sf::Vector2f(420.f, 70.f));
    boton.setOrigin(boton.getSize().x / 2.f, boton.getSize().y / 2.f);
    boton.setPosition(x, y);

    if (seleccionado) {
        boton.setFillColor(sf::Color(180, 40, 40));
    }
    else {
        boton.setFillColor(sf::Color(60, 60, 60));
    }

    boton.setOutlineThickness(3.f);
    boton.setOutlineColor(sf::Color::White);
}

    
OpcionMenu Menu::mostrarMenuPrincipal() {
   
    std::string opciones[3] = {
        "Nueva Partida",
        "Continuar Partida",
        "Salir"
        };

        sf::Text titulo;
        titulo.setFont(fuente);
        titulo.setString("ZWAVE");
        titulo.setCharacterSize(64);
        titulo.setFillColor(sf::Color(220, 40, 40));
        sf::FloatRect tituloBounds = titulo.getLocalBounds();

        titulo.setOrigin(
            tituloBounds.left + tituloBounds.width / 2.f,
            tituloBounds.top + tituloBounds.height / 2.f
        );
          
        titulo.setPosition(640.f, 130.f);


        sf::Text textos[3];

        textos[0].setString("Nueva Partida");
        textos[1].setString("Continuar Partida");
        textos[2].setString("Salir");

        for (int i = 0; i < 3; i++) {
            textos[i].setFont(fuente);
            textos[i].setCharacterSize(30);
            textos[i].setFillColor(sf::Color::White);
            sf::FloatRect bounds = textos[i].getLocalBounds();

            textos[i].setOrigin(
                bounds.left + bounds.width / 2.f,
                bounds.top + bounds.height / 2.f
            );

            textos[i].setPosition(
                640.f,
                300.f + i * 90.f
            );
        }

        while (ventana.isOpen()) {
            sf::Event evento;

            while (ventana.pollEvent(evento)) {
                if (evento.type == sf::Event::Closed) {
                    ventana.close();
                    return OpcionMenu::Salir;
                }

                if (evento.type == sf::Event::KeyPressed) {
                    if (evento.key.code == sf::Keyboard::Escape) {
                        ventana.close();
                        return OpcionMenu::Salir;
                    }

                    if (evento.key.code == sf::Keyboard::W || evento.key.code == sf::Keyboard::Up) {
                        opcionActual--;
                        if (opcionActual < 0) opcionActual = 2;
                    }

                    if (evento.key.code == sf::Keyboard::S || evento.key.code == sf::Keyboard::Down) {
                        opcionActual++;
                        if (opcionActual > 2) opcionActual = 0;
                    }

                    if (evento.key.code == sf::Keyboard::Enter) {
                        ventana.close();

                        if (opcionActual == 0) return OpcionMenu::NuevaPartida;
                        if (opcionActual == 1) return OpcionMenu::ContinuarPartida;

                        return OpcionMenu::Salir;
                    }
                }
            }

            ventana.clear(sf::Color(20, 20, 20));

            for (int i = 0; i < 3; i++)
            {
                if (i == opcionActual)
                {
                    textos[i].setFillColor(sf::Color::Yellow);
                    textos[i].setString("> " + opciones[i] + " <");
                }
                else
                {
                    textos[i].setFillColor(sf::Color(200, 200, 200));
                    textos[i].setString(opciones[i]);
                }

                sf::FloatRect bounds = textos[i].getLocalBounds();

                textos[i].setOrigin(
                    bounds.left + bounds.width / 2.f,
                    bounds.top + bounds.height / 2.f
                );

                textos[i].setPosition(
                    640.f,
                    300.f + i * 90.f
                );

                ventana.draw(textos[i]);
            }

            ventana.draw(titulo);


            ventana.display();
        }

        return OpcionMenu::Salir;
    }
   
TipoPersonaje Menu::mostrarSeleccionPersonaje() {
    sf::VideoMode modo = sf::VideoMode::getDesktopMode();
    ventana.create(modo, "Seleccion de Personaje", sf::Style::Fullscreen);
    ventana.setFramerateLimit(60);
    ventana.setView(vista);

    sf::RectangleShape botones[3];

    while (ventana.isOpen()) {
        sf::Event evento;

        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                ventana.close();
                return TipoPersonaje::Superviviente;
            }

            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Escape) {
                    ventana.close();
                    return TipoPersonaje::Superviviente;
                }

                if (evento.key.code == sf::Keyboard::W || evento.key.code == sf::Keyboard::Up) {
                    personajeActual--;
                    if (personajeActual < 0) personajeActual = 2;
                }

                if (evento.key.code == sf::Keyboard::S || evento.key.code == sf::Keyboard::Down) {
                    personajeActual++;
                    if (personajeActual > 2) personajeActual = 0;
                }

                if (evento.key.code == sf::Keyboard::Enter) {
                    ventana.close();
                    return static_cast<TipoPersonaje>(personajeActual);
                }
            }
        }

        ventana.clear(sf::Color(15, 15, 30));

       




        ventana.display();
    }

    return TipoPersonaje::Superviviente;
}


OpcionPausa Menu::mostrarMenuPausa(sf::RenderWindow & ventanaJuego) {
        int opcion = 0;

        sf::View vistaAnterior = ventanaJuego.getView();
        ventanaJuego.setView(ventanaJuego.getDefaultView());

        std::string opciones[2] = {
            "Seguir jugando",
            "Salir"
        };

        while (ventanaJuego.isOpen()) {
            sf::Event evento;

            while (ventanaJuego.pollEvent(evento)) {
                if (evento.type == sf::Event::Closed) {
                    ventanaJuego.setView(vistaAnterior);
                    return OpcionPausa::Salir;
                }

                if (evento.type == sf::Event::KeyPressed) {
                    if (evento.key.code == sf::Keyboard::Escape) {
                        ventanaJuego.setView(vistaAnterior);
                        return OpcionPausa::Seguir;
                    }

                    if (evento.key.code == sf::Keyboard::W || evento.key.code == sf::Keyboard::Up) {
                        opcion--;
                        if (opcion < 0) opcion = 1;
                    }

                    if (evento.key.code == sf::Keyboard::S || evento.key.code == sf::Keyboard::Down) {
                        opcion++;
                        if (opcion > 1) opcion = 0;
                    }

                    if (evento.key.code == sf::Keyboard::Enter) {
                        ventanaJuego.setView(vistaAnterior);

                        if (opcion == 0) {
                            return OpcionPausa::Seguir;
                        }

                        return OpcionPausa::Salir;
                    }
                }
            }

            ventanaJuego.clear(sf::Color(20, 20, 20));

            sf::RectangleShape fondo;
            fondo.setSize(sf::Vector2f(
                static_cast<float>(ventanaJuego.getSize().x),
                static_cast<float>(ventanaJuego.getSize().y)
            ));
            fondo.setFillColor(sf::Color(0, 0, 0, 170));
            ventanaJuego.draw(fondo);

            sf::Text titulo;
            titulo.setFont(fuente);
            titulo.setString("PAUSA");
            titulo.setCharacterSize(64);
            titulo.setFillColor(sf::Color(220, 40, 40));

            sf::FloatRect boundsTitulo = titulo.getLocalBounds();
            titulo.setOrigin(
                boundsTitulo.left + boundsTitulo.width / 2.f,
                boundsTitulo.top + boundsTitulo.height / 2.f
            );

            titulo.setPosition(
                ventanaJuego.getSize().x / 2.f,
                ventanaJuego.getSize().y / 2.f - 120.f
            );

            ventanaJuego.draw(titulo);

            for (int i = 0; i < 2; i++) {
                sf::Text texto;
                texto.setFont(fuente);
                texto.setCharacterSize(34);

                if (i == opcion) {
                    texto.setString("> " + opciones[i] + " <");
                    texto.setFillColor(sf::Color::Yellow);
                }
                else {
                    texto.setString(opciones[i]);
                    texto.setFillColor(sf::Color(220, 220, 220));
                }

                sf::FloatRect bounds = texto.getLocalBounds();
                texto.setOrigin(
                    bounds.left + bounds.width / 2.f,
                    bounds.top + bounds.height / 2.f
                );

                texto.setPosition(
                    ventanaJuego.getSize().x / 2.f,
                    ventanaJuego.getSize().y / 2.f + i * 80.f
                );

                ventanaJuego.draw(texto);
            }

            ventanaJuego.display();
        }

        ventanaJuego.setView(vistaAnterior);
        return OpcionPausa::Salir;
    }
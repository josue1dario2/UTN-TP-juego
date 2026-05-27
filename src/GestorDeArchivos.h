#pragma once

// ============================================================================
// GestorDeArchivos.h - Gestor de archivos de datos del juego
// ============================================================================
// Este módulo se encarga de leer y escribir archivos .dat en formato CSV.
// Maneja tres tipos de datos:
//   - Personajes (Personaje): stats de cada personaje jugable
//   - Armas (ArmaDatos): propiedades de cada arma del juego
//   - Estadísticas (Estadistica): récords de partidas jugadas
//
// Todos los métodos son estáticos (static) porque no necesitamos crear
// instancias de GestorDeArchivos — solo llamamos sus funciones directamente.
// Ejemplo: GestorDeArchivos::cargarPersonajes("data/Personajes.dat");
// ============================================================================

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Personaje.h"
#include "ArmaDatos.h"
#include "Estadistica.h"
#include "Partida.h"

// ============================================================================
// Clase GestorDeArchivos - Todos los métodos son estáticos
// ============================================================================

class GestorDeArchivos {
public:
    // Lee el archivo de personajes y retorna un vector con los datos.
    // Formato CSV: id,nombre,vida,armadura,velocidad,habilidad
    static std::vector<Personaje> cargarPersonajes(const std::string& filepath);

    // Lee el archivo de armas y retorna un vector con los datos.
    // Formato CSV: id,nombre,danoBase,cadencia,alcance,tipo,tamanoMagazine,municionTotal
    static std::vector<ArmaDatos> cargarArmas(const std::string& filepath);

    // Agrega una entrada de estadísticas al final del archivo.
    // Formato CSV: nombreJugador,idPersonaje,puntuacion,tiempoJugado
    static void guardarEstadistica(const std::string& filepath, const Estadistica& entrada);

    // Lee todas las entradas de estadísticas del archivo.
    static std::vector<Estadistica> cargarEstadisticas(const std::string& filepath);

    // Guarda una partida completa en archivo.
    // Formato CSV: idPartida,oleadaActual,dinero,puntos,tiempo,idPersonaje,nombrePersonaje
    static void guardarPartida(const std::string& filepath, const Partida& partida);

    // Carga una partida desde archivo.
    static Partida cargarPartida(const std::string& filepath);
};
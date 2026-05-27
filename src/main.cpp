/**
 * @file main.cpp
 * @brief Punto de entrada del juego Z-Wave.
 *
 * Este archivo es INTENCIONALMENTE simple. Siguiendo el principio de
 * RESPONSABILIDAD ÚNICA (Single Responsibility Principle), main() solo
 * crea el objeto Game y lo ejecuta. Toda la lógica está en la clase Game.
 *
 * En C++, la función main() es la PRIMERA función que se ejecuta cuando
 * arrancamos el programa. Retorna 0 si todo salió bien, o un número
 * distinto si hubo un error.
 */

#include "Juego.h"

int main() {
    // Creamos la instancia del juego
    // El constructor de Juego inicializa la ventana y todos los sistemas
    Juego juego;

    // Ejecutamos el bucle principal del juego
    // ejecutar() no retorna hasta que el jugador cierra la ventana
    juego.ejecutar();

    // Retornamos 0 indicando que el programa terminó correctamente
    return 0;
}

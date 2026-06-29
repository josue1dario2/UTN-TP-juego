#include "../include/Juego.h"
#include "../include/menuDesarrollador.h"
#include "../include/archivoPersonaje.h"

int main()
{
    bool modoDev = false;

    if (modoDev == false) {
        Juego juego;
        juego.iniciar();
    }
    else {
        // Iniciar el modo desarrollador para gestionar armas y personajes
        MenuDesarrollador menuDesarrollador;
        menuDesarrollador.iniciarModoDesarrollador();
    }
    return 0;
}
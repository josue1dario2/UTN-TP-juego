#include "../include/Juego.h"
#include "../include/menuDesarrollador.h"

int main()
{
    // la clase juego se encarga de correr todo en el main solo iniciamos
    Juego juego;
    juego.iniciar();

    // Iniciar el modo desarrollador para gestionar armas y personajes
    //MenuDesarrollador menuDesarrollador;
    //menuDesarrollador.iniciarModoDesarrollador();

    return 0;
}
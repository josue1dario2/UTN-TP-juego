#include "../include/Juego.h"
#include "../include/Menu.h"

int main() {
  
        Menu menu;

        OpcionMenu opcion = menu.mostrarMenuPrincipal();

        if (opcion == OpcionMenu::Salir)
        {
            return 0;
        }

        if (opcion == OpcionMenu::ContinuarPartida)
        {
            // Por ahora continuar partida entra igual que nueva partida.
            // Después acá cargamos el archivo de guardado.
        }

        Juego juego;
        juego.iniciar();

        return 0;
    }
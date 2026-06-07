#include "../include/MenuDesarrollador.h"
#include "../include/archivoArma.h"

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void MenuDesarrollador::iniciarModoDesarrollador() {
    menuPrincipal();
}

void MenuDesarrollador::menuArmas() {
    bool flag = true;

    while (flag) {
        limpiarPantalla();
        std::cout << "------- Menu armas -------" << std::endl;
        std::cout << "1. Nuevo Registro" << std::endl;
        std::cout << "2. Listar Armas" << std::endl;
        std::cout << "3. Modificar Arma" << std::endl;
        std::cout << "0. Volver al menu principal" << std::endl;

        archivoArma archivo("armas.dat");
        
        int opcion;
        std::cin >> opcion;
        
        switch (opcion) {
            case 0:
                flag = false;
                break;
            
            case 1: {

                limpiarPantalla();
                std::cout << "----- Nuevo Registro -----" << std::endl;
                
                int id;
                char nombre[50];
                float cadencia, danio, alcance, costo;
                int municionMaxima, tamanioCargador;
                
                std::cout << "Ingrese el ID del arma: ";
                std::cin >> id;
                std::cout << "Ingrese el nombre del arma: ";
                std::cin >> nombre;
                std::cout << "Ingrese la cadencia del arma: ";
                std::cin >> cadencia;
                std::cout << "Ingrese el daño del arma: ";
                std::cin >> danio;
                std::cout << "Ingrese el alcance del arma: ";
                std::cin >> alcance;
                std::cout << "Ingrese el costo del arma: ";
                std::cin >> costo;
                std::cout << "Ingrese la munición máxima del arma: ";
                std::cin >> municionMaxima;
                std::cout << "Ingrese el tamaño del cargador del arma: ";
                std::cin >> tamanioCargador;
                
                RegistroArma arma;
                
                arma.id = id;
                std::strcpy(arma.nombre, nombre);
                arma.cadencia = cadencia;
                arma.danio = danio;
                arma.alcance = alcance;
                arma.costo = costo;
                arma.municionMaxima = municionMaxima;
                arma.tamanioCargador = tamanioCargador;
                
                archivo.grabarRegistroArma(arma);
                
                break;
            }
            
            case 2: {
                limpiarPantalla();
                std::cout << "Listado de armas: " << std::endl;
                archivo.listar();
                std::cout << "Presiona Enter para continuar...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
            
            case 3: {

                limpiarPantalla();
                std::cout << "----- Modificar arma -----" << std::endl;
                archivo.listar();
                int idMod = -1;
                std::cout << "Ingrese el ID del arma a modificar: ";
                std::cin >> idMod;
                if(idMod < 0) {
                    std::cout << "ID no valido" << std::endl;
                    break;
                }
                int posicion = archivo.buscarArma(idMod);
                if(posicion == -1) {
                    std::cout << "No se encontro el arma con ID " << idMod << std::endl;
                    std::cout << "Presiona Enter para continuar...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                RegistroArma armaModificar = archivo.traerRegistro(posicion);

                std::cout << "Nombre actual: " << armaModificar.nombre << std::endl;
                std::cout << "Ingrese el nuevo nombre del arma: ";
                std::cin >> armaModificar.nombre;

                std::cout << "Cadencia actual: " << armaModificar.cadencia << std::endl;
                std::cout << "Ingrese la nueva cadencia del arma: ";
                std::cin >> armaModificar.cadencia;

                std::cout << "Daño actual: " << armaModificar.danio << std::endl;
                std::cout << "Ingrese el nuevo daño del arma: ";
                std::cin >> armaModificar.danio;

                std::cout << "Alcance actual: " << armaModificar.alcance << std::endl;
                std::cout << "Ingrese el nuevo alcance del arma: ";
                std::cin >> armaModificar.alcance;

                std::cout << "Costo actual: " << armaModificar.costo << std::endl;
                std::cout << "Ingrese el nuevo costo del arma: ";
                std::cin >> armaModificar.costo;

                std::cout << "Munición máxima actual: " << armaModificar.municionMaxima << std::endl;
                std::cout << "Ingrese la nueva munición máxima del arma: ";
                std::cin >> armaModificar.municionMaxima;

                std::cout << "Tamaño del cargador actual: " << armaModificar.tamanioCargador << std::endl;
                std::cout << "Ingrese el nuevo tamaño del cargador del arma: ";
                std::cin >> armaModificar.tamanioCargador;

                std::cout << "Confirmar modificación? (1 = si, 0 = no): ";
                int confirmar;
                std::cin >> confirmar;
                if(confirmar != 1) {
                    std::cout << "Modificación cancelada" << std::endl;
                    break;
                }

                if(archivo.sobreEscribirRegistro(armaModificar, posicion)) {
                    std::cout << "Modificacion exitosa" << std::endl;
                } else {
                    std::cout << "Error al modificar el arma" << std::endl;
                }

                std::cout << "Presiona Enter para continuar...";
                std::cin.ignore();
                std::cin.get();


                break;
            }
            
            default: {
                std::cout << "Opcion no valida, vuelva a intentarlo" << std::endl;
                break;
            }
        }
    }
}

void MenuDesarrollador::menuPersonajes() {
    // Implementación similar a menuArmas, pero para personajes
    // Debería permitir crear, listar y modificar personajes utilizando un archivo similar a archivoArma
    }

/*
void MenuDesarrollador::menuPersonajes() {
    
    bool flag = true;
    
    while (flag) {
        limpiarPantalla();
        std::cout << "------- Menu personajes -------" << std::endl;
        std::cout << "1. Nuevo Registro" << std::endl;
        std::cout << "2. Listar Personajes" << std::endl;
        std::cout << "3. Modificar Personaje" << std::endl;
        std::cout << "0. Volver al menu principal" << std::endl;

        archivoPersonaje archivo("personajes.dat");
        
        int opcion;
        std::cin >> opcion;
        
        switch (opcion) {
            case 0:
                flag = false;
                break;
            
            case 1: {

                limpiarPantalla();
                std::cout << "----- Nuevo Registro -----" << std::endl;
                
                int idPersonaje, idArmaEspecial;
                char nombre[50];
                float vida, armadura, velocidad;
                
                std::cout << "Ingrese el ID del personaje: ";
                std::cin >> idPersonaje;
                std::cout << "Ingrese el ID del arma especial: ";
                std::cin >> idArmaEspecial;
                std::cout << "Ingrese el nombre: ";
                std::cin >> nombre;
                std::cout << "Ingrese la vida del personaje: ";
                std::cin >> vida;
                std::cout << "Ingrese la armadura del personaje: ";
                std::cin >> armadura;
                std::cout << "Ingrese la velocidad del personaje: ";
                std::cin >> velocidad;
                
                RegistroPersonaje personaje;
                
                personaje.id = idPersonaje;
                personaje.idArmaEspecial = idArmaEspecial;
                std::strcpy(personaje.nombre, nombre);
                personaje.vida = vida;
                personaje.armadura = armadura;
                personaje.velocidad = velocidad;
                
                archivo.grabarRegistroPersonaje(personaje);
                
                break;
            }
            
            case 2: {
                limpiarPantalla();
                std::cout << "Listado de personajes: " << std::endl;
                archivo.listar();
                std::cout << "Presiona Enter para continuar...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
            
            case 3: {

                limpiarPantalla();
                std::cout << "----- Modificar personaje -----" << std::endl;
                archivo.listar();
                int idMod = -1;
                std::cout << "Ingrese el ID del personaje a modificar: ";
                std::cin >> idMod;
                if(idMod < 0) {
                    std::cout << "ID no valido" << std::endl;
                    break;
                }
                int posicion = archivo.buscarPersonaje(idMod);
                if(posicion == -1) {
                    std::cout << "No se encontro el personaje con ID " << idMod << std::endl;
                    std::cout << "Presiona Enter para continuar...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                RegistroPersonaje personajeModificar = archivo.traerRegistro(posicion);

                std::cout << "Nombre actual: " << personajeModificar.nombre << std::endl;
                std::cout << "Ingrese el nuevo nombre del arma: ";
                std::cin >> armaModificar.nombre;

                std::cout << "Cadencia actual: " << armaModificar.cadencia << std::endl;
                std::cout << "Ingrese la nueva cadencia del arma: ";
                std::cin >> armaModificar.cadencia;

                std::cout << "Daño actual: " << armaModificar.danio << std::endl;
                std::cout << "Ingrese el nuevo daño del arma: ";
                std::cin >> armaModificar.danio;

                std::cout << "Alcance actual: " << armaModificar.alcance << std::endl;
                std::cout << "Ingrese el nuevo alcance del arma: ";
                std::cin >> armaModificar.alcance;

                std::cout << "Costo actual: " << armaModificar.costo << std::endl;
                std::cout << "Ingrese el nuevo costo del arma: ";
                std::cin >> armaModificar.costo;

                std::cout << "Munición máxima actual: " << armaModificar.municionMaxima << std::endl;
                std::cout << "Ingrese la nueva munición máxima del arma: ";
                std::cin >> armaModificar.municionMaxima;

                std::cout << "Confirmar modificación? (1 = si, 0 = no): ";
                int confirmar;
                std::cin >> confirmar;
                if(confirmar != 1) {
                    std::cout << "Modificación cancelada" << std::endl;
                    break;
                }

                if(archivo.sobreEscribirRegistro(armaModificar, posicion)) {
                    std::cout << "Modificacion exitosa" << std::endl;
                } else {
                    std::cout << "Error al modificar el arma" << std::endl;
                }

                std::cout << "Presiona Enter para continuar...";
                std::cin.ignore();
                std::cin.get();


                break;
            }
            
            default: {
                std::cout << "Opcion no valida, vuelva a intentarlo" << std::endl;
                break;
            }
        }
    }
}
*/

void MenuDesarrollador::menuPrincipal() {
    bool flag = true;

    while (flag) {
        limpiarPantalla();
        std::cout << "------------ Modo desarrollador iniciado ------------" << std::endl;
        std::cout << "1. Menu armas" << std::endl;
        std::cout << "2. Menu personajes" << std::endl;
        std::cout << "0. Salir" << std::endl;
        std::cout << "Elija una opcion: ";
        int opcion;
        std::cin >> opcion;
        
        switch (opcion) {
            case 0:
                flag = false;
                break;
            case 1:
                menuArmas();
                break;
            case 2:
                menuPersonajes();
                break;
            default:
                std::cout << "Opcion no valida, vuelva a intentarlo" << std::endl;
                break;
        }
    }
}
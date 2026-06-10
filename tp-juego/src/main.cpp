#include "../include/Juego.h"
#include "../include/menuDesarrollador.h"
#include "../include/archivoPersonaje.h"

int main()
{
    archivoPersonaje archivo("personajes.dat");
    int id = 0;
    
    int pos = -1;
    RegistroPersonaje registro;
    
    pos = archivo.buscarPersonaje(id);
    registro = archivo.traerRegistro(pos);
    
    std::cout << "----- THE LAST SQUAD -----" << std::endl;
    std::cout << "Seleccione un personaje: ID 0" << std::endl;
    
    std::cout << registro.id << std::endl;    
    std::cout << registro.idArmaEspecial << std::endl;
    std::cout << registro.nombre << std::endl;
    std::cout << registro.vida << std::endl;
    std::cout << registro.armadura << std::endl;
    std::cout << registro.velocidad << std::endl;
    std::cout << registro.cooldownHabilidad << std::endl;
    
    std::cout << "Presiona Enter para continuar...";
    std::cin.ignore();
    std::cin.get();
    



    // la clase juego se encarga de correr todo en el main solo iniciamos
    Juego juego{registro.id,registro.idArmaEspecial, registro.nombre, registro.vida, registro.armadura, registro.velocidad, registro.cooldownHabilidad};
    juego.iniciar();

    // Iniciar el modo desarrollador para gestionar armas y personajes


    //MenuDesarrollador menuDesarrollador;
    //menuDesarrollador.iniciarModoDesarrollador();

    return 0;
}
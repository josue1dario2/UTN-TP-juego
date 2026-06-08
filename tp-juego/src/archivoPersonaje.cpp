#include "../include/archivoPersonaje.h"


//_nombre es un arreglo de char

archivoPersonaje::archivoPersonaje(const char* nombre){
    strcpy(_nombre,nombre);
    verificarArchivo(_nombre);
}

bool archivoPersonaje::verificarArchivo(const char* nombreArchivo)
{
    FILE* pArchivo = fopen(nombreArchivo, "rb");

    if(pArchivo == NULL)
    {
        // El archivo no existe, lo creamos
        pArchivo = fopen(nombreArchivo, "wb");

        if(pArchivo == NULL)
        {
            return false;
        }
    }

    fclose(pArchivo);
    return true;
}

// registrar arma nueva, devuelve true si se pudo grabar, false si hubo un error
bool archivoPersonaje::grabarRegistroPersonaje (RegistroPersonaje objeto) {

    FILE *pPersonaje;

    pPersonaje = fopen (_nombre, "ab");

    if(pPersonaje == nullptr) {
        std::cout << "ERROR DE ARCHIVO" << std::endl;
        return false;
    }

    bool escribio = fwrite ( &objeto, sizeof(objeto), 1, pPersonaje);

    fclose(pPersonaje);

    return escribio;
}

// 1- buscar la pocision de un arma segun el id
int archivoPersonaje::buscarPersonaje(int idPersonaje) {
    RegistroPersonaje registro;

    int posicion = 0;

    FILE *pPersonaje = fopen(_nombre, "rb");

    if(pPersonaje == nullptr) {
        std::cout << "ERROR DE ARCHIVO" << std::endl;
        return -1;
    }

    while (fread (&registro, sizeof (registro), 1, pPersonaje) == 1) {
        if (idPersonaje == registro.id) {
            fclose(pPersonaje);
            return posicion;
        }
        posicion++;
    }

    fclose(pPersonaje);

    return -1;
}

// 3- sobreescribimos el registro despues de hacer los cambios
bool archivoPersonaje::sobreEscribirRegistro(RegistroPersonaje registro, int posicion) {

    FILE *pPersonaje = fopen (_nombre, "rb+");///+ le agrega al modo lo que le falta

    if(pPersonaje == nullptr) {
        std::cout << "ERROR DE ARCHIVO" << std::endl;
        return false;
    }

    fseek (pPersonaje, posicion*sizeof (registro), 0); ///SEEK_SET->0 PRINCIPIO; SEEK_CUR->1 POSICION ACTUAL; SEEK_END->2 FINAL DEL ARCHIVO

    bool escribio = fwrite (&registro, sizeof (registro), 1, pPersonaje);

    fclose(pPersonaje);

    return escribio;
}

// 2- busca el registro segun la posicion y devuelve el registro
// arma por default es con id -1
RegistroPersonaje archivoPersonaje::traerRegistro (int posicion) {
    RegistroPersonaje registro;

    FILE *pPersonaje = fopen(_nombre, "rb");

    if(pPersonaje == nullptr) {
        std::cout << "ERROR DE ARCHIVO" << std::endl;
        return registro;
    }

    fseek (pPersonaje, posicion*sizeof (registro), 0); ///SEEK_SET->0 PRINCIPIO; SEEK_CUR->1 POSICION ACTUAL; SEEK_END->2 FINAL DEL ARCHIVO

    fread (&registro, sizeof (registro), 1, pPersonaje);

    fclose(pPersonaje);

    return registro;
}

// listar los nombres
void archivoPersonaje::listar() {
    RegistroPersonaje objeto;

    FILE *pPersonaje;

    pPersonaje = fopen (_nombre,"rb");

    if(pPersonaje == nullptr){
        std::cout << "ERROR DE ARCHIVO" << std::endl;
        return;
    }

    while (fread (&objeto, sizeof (objeto), 1, pPersonaje) != 0) {
        std::cout << "-ID " << objeto.id << " - " << objeto.nombre << std::endl;
    }

    fclose (pPersonaje);
}

/*
void archivoPersonaje::entregarArma(std::vector<Arma>& inventario, int idArma) {
    int posicion = buscarArma(idArma);
    
    if(posicion == -1) {
        std::cout << "ARMA NO ENCONTRADA" << std::endl;
        inventario.emplace_back(); // Agrega un arma vacía al inventario para mantener la consistencia
        return;
    }
    
    RegistroArma registro = traerRegistro(posicion);
    
    inventario.emplace_back(registro.id, registro.nombre, registro.cadencia, registro.danio, registro.alcance, registro.costo, registro.municionMaxima, registro.tamanioCargador);
}
*/
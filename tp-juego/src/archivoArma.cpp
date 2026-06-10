#include "../include/archivoArma.h"


//_nombre es un arreglo de char

archivoArma::archivoArma(const char* nombre){
    strcpy(_nombre,nombre);
    verificarArchivo(_nombre);
}

bool archivoArma::verificarArchivo(const char* nombreArchivo)
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
bool archivoArma::grabarRegistroArma (RegistroArma objeto) {

    FILE *pArma;

    pArma = fopen (_nombre, "ab");

    if(pArma == nullptr) {
        std::cout << "ERROR DE ARCHIVO" << std::endl;
        return false;
    }

    bool escribio = fwrite ( &objeto, sizeof(objeto), 1, pArma);

    fclose(pArma);

    return escribio;
}

// 1- buscar la pocision de un arma segun el id
int archivoArma::buscarArma(int idArma) {
    RegistroArma registro;

    int posicion = 0;

    FILE *pArma = fopen(_nombre, "rb");

    if(pArma == nullptr) {
        std::cout << "ERROR DE ARCHIVO" << std::endl;
        return -1;
    }

    while (fread (&registro, sizeof (registro), 1, pArma) == 1) {
        if (idArma == registro.id) {
            fclose(pArma);
            return posicion;
        }
        posicion++;
    }

    fclose(pArma);

    return -1;
}

// 3- sobreescribimos el registro despues de hacer los cambios
bool archivoArma::sobreEscribirRegistro(RegistroArma registro, int posicion) {

    FILE *pArma = fopen (_nombre, "rb+");///+ le agrega al modo lo que le falta

    if(pArma == nullptr) {
        std::cout << "ERROR DE ARCHIVO" << std::endl;
        return false;
    }

    fseek (pArma, posicion*sizeof (registro), 0); ///SEEK_SET->0 PRINCIPIO; SEEK_CUR->1 POSICION ACTUAL; SEEK_END->2 FINAL DEL ARCHIVO

    bool escribio = fwrite (&registro, sizeof (registro), 1, pArma);

    fclose(pArma);

    return escribio;
}

// 2- busca el registro segun la posicion y devuelve el registro
// arma por default es con id -1
RegistroArma archivoArma::traerRegistro (int posicion) {
    RegistroArma registro;

    FILE *pArma = fopen(_nombre, "rb");

    if(pArma == nullptr) {
        std::cout << "ERROR DE ARCHIVO" << std::endl;
        return registro;
    }

    fseek (pArma, posicion*sizeof (registro), 0); ///SEEK_SET->0 PRINCIPIO; SEEK_CUR->1 POSICION ACTUAL; SEEK_END->2 FINAL DEL ARCHIVO

    fread (&registro, sizeof (registro), 1, pArma);

    fclose(pArma);

    return registro;
}

// listar los nombres
void archivoArma::listar() {
    RegistroArma objeto;

    FILE *pArma;

    pArma = fopen (_nombre,"rb");

    if(pArma == nullptr){
        std::cout << "ERROR DE ARCHIVO" << std::endl;
        return;
    }

    while (fread (&objeto, sizeof (objeto), 1, pArma) != 0) {
        std::cout << "-ID " << objeto.id << " - " << objeto.nombre << std::endl;
    }

    fclose (pArma);
}

void archivoArma::entregarArma(std::vector<Arma>& inventario, int idArma) {
    int posicion = buscarArma(idArma);

    if(posicion == -1) {
        std::cout << "ARMA NO ENCONTRADA" << std::endl;
        inventario.emplace_back(); // Agrega un arma vacía al inventario para mantener la consistencia
        return;
    }

    RegistroArma registro = traerRegistro(posicion);

    inventario.emplace_back(registro.id, registro.nombre, registro.cadencia, registro.danio, registro.alcance, registro.costo, registro.municionMaxima, registro.tamanioCargador);
}
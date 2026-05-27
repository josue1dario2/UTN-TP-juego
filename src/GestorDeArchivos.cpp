// ============================================================================
// GestorDeArchivos.cpp - Implementación del gestor de archivos
// ============================================================================
// Aquí se implementa la lectura y escritura de archivos CSV (.dat).
//
// Técnicas de parsing utilizadas:
//   - std::getline() para leer líneas completas
//   - std::istringstream para parsear campos
//   - std::stoi() / std::stof() para convertir texto a números
//
// Manejo de errores:
//   - Si el archivo no existe, se retorna un vector vacío (sin crashear)
//   - Si una línea está mal formateada, se salta y se continúa
// ============================================================================

#include "GestorDeArchivos.h"

// ============================================================================
// Función auxiliar privada para separar una línea CSV en campos
// ============================================================================
static std::vector<std::string> splitCSV(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    std::istringstream stream(line);

    while (std::getline(stream, field, ',')) {
        fields.push_back(field);
    }

    return fields;
}

// ============================================================================
// cargarPersonajes - Cargar personajes desde archivo
// ============================================================================
// Formato esperado: id,nombre,vida,armadura,velocidad,habilidad
// Ejemplo: 1,Tank,150,100,150,Escudo Temporal
std::vector<Personaje> GestorDeArchivos::cargarPersonajes(const std::string& filepath) {
    std::vector<Personaje> personajes;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "[GestorDeArchivos] ADVERTENCIA: No se pudo abrir el archivo de personajes: "
                  << filepath << std::endl;
        return personajes;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> fields = splitCSV(line);

        if (fields.size() < 6) {
            std::cerr << "[GestorDeArchivos] ADVERTENCIA: Línea mal formateada en personajes: "
                      << line << std::endl;
            continue;
        }

        try {
            Personaje p;
            p.setId(std::stoi(fields[0]));
            p.setNombre(fields[1]);
            p.setVidaInicial(std::stoi(fields[2]));
            p.setArmaduraInicial(std::stoi(fields[3]));
            p.setVelocidadInicial(std::stoi(fields[4]));
            p.setHabilidad(fields[5]);

            personajes.push_back(p);
        } catch (const std::exception& e) {
            std::cerr << "[GestorDeArchivos] Error al parsear personaje: " << e.what()
                      << " en línea: " << line << std::endl;
        }
    }

    std::cout << "[GestorDeArchivos] " << personajes.size() << " personajes cargados." << std::endl;
    return personajes;
}

// ============================================================================
// cargarArmas - Cargar armas desde archivo
// ============================================================================
// Formato esperado: id,nombre,danoBase,cadencia,alcance,tipo,tamanoMagazine,municionTotal
// Ejemplo: 1,Pistola,15,0.4,400,pistola,12,60
std::vector<ArmaDatos> GestorDeArchivos::cargarArmas(const std::string& filepath) {
    std::vector<ArmaDatos> armas;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "[GestorDeArchivos] ADVERTENCIA: No se pudo abrir el archivo de armas: "
                  << filepath << std::endl;
        return armas;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> fields = splitCSV(line);

        if (fields.size() < 8) {
            std::cerr << "[GestorDeArchivos] ADVERTENCIA: Línea mal formateada en armas: "
                      << line << std::endl;
            continue;
        }

        try {
            ArmaDatos a;
            a.setId(std::stoi(fields[0]));
            a.setNombre(fields[1]);
            a.setDanoBase(std::stoi(fields[2]));
            a.setCadencia(std::stof(fields[3]));
            a.setAlcance(std::stof(fields[4]));
            a.setTipo(fields[5]);
            a.setTamanoMagazine(std::stoi(fields[6]));
            a.setMunicionTotal(std::stoi(fields[7]));

            armas.push_back(a);
        } catch (const std::exception& e) {
            std::cerr << "[GestorDeArchivos] Error al parsear arma: " << e.what()
                      << " en línea: " << line << std::endl;
        }
    }

    std::cout << "[GestorDeArchivos] " << armas.size() << " armas cargadas." << std::endl;
    return armas;
}

// ============================================================================
// guardarEstadistica - Guardar una entrada de estadísticas
// ============================================================================
// Abre el archivo en modo "append" (agregar al final) para no borrar
// las estadísticas anteriores.
void GestorDeArchivos::guardarEstadistica(const std::string& filepath, const Estadistica& entrada) {
    std::ofstream file(filepath, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "[GestorDeArchivos] ERROR: No se pudo abrir el archivo de estadísticas para escritura: "
                  << filepath << std::endl;
        return;
    }

    file << entrada.aCSV() << "\n";

    std::cout << "[GestorDeArchivos] Estadísticas guardadas para: " 
              << entrada.getNombreJugador() << std::endl;
}

// ============================================================================
// cargarEstadisticas - Cargar todas las estadísticas
// ============================================================================
// Formato esperado: nombreJugador,idPersonaje,puntuacion,tiempoJugado
// Ejemplo: Jugador1,1,5000,300
std::vector<Estadistica> GestorDeArchivos::cargarEstadisticas(const std::string& filepath) {
    std::vector<Estadistica> estadisticas;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "[GestorDeArchivos] ADVERTENCIA: No se pudo abrir el archivo de estadísticas: "
                  << filepath << std::endl;
        return estadisticas;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        try {
            estadisticas.push_back(Estadistica::desdeCSV(line));
        } catch (const std::exception& e) {
            std::cerr << "[GestorDeArchivos] Error al parsear estadística: " << e.what()
                      << " en línea: " << line << std::endl;
        }
    }

    std::cout << "[GestorDeArchivos] " << estadisticas.size() << " entradas de estadísticas cargadas." << std::endl;
    return estadisticas;
}

// ============================================================================
// guardarPartida - Guardar una partida completa
// ============================================================================
// Formato: idPartida,oleadaActual,dinero,puntos,tiempo,idPersonaje,nombrePersonaje
void GestorDeArchivos::guardarPartida(const std::string& filepath, const Partida& partida) {
    std::ofstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "[GestorDeArchivos] ERROR: No se pudo abrir el archivo de partida para escritura: "
                  << filepath << std::endl;
        return;
    }

    file << partida.aCSV() << "\n";

    std::cout << "[GestorDeArchivos] Partida guardada (Oleada: " 
              << partida.getOleadaActual() << ", Puntos: " << partida.getPuntos() << ")" << std::endl;
}

// ============================================================================
// cargarPartida - Cargar una partida desde archivo
// ============================================================================
Partida GestorDeArchivos::cargarPartida(const std::string& filepath) {
    std::ifstream file(filepath);
    Partida partidaVacia;

    if (!file.is_open()) {
        std::cerr << "[GestorDeArchivos] ADVERTENCIA: No se pudo abrir el archivo de partida: "
                  << filepath << std::endl;
        return partidaVacia;
    }

    std::string line;

    // Solo leemos la primera línea (la última partida guardada)
    if (std::getline(file, line)) {
        if (!line.empty()) {
            std::cout << "[GestorDeArchivos] Partida cargada desde: " << filepath << std::endl;
            return Partida::desdeCSV(line);
        }
    }

    return partidaVacia;
}
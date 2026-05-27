// ============================================================================
// GestorRecursos.cpp - Implementación del gestor de recursos
// ============================================================================
// Aquí se implementan todos los métodos de carga y acceso a recursos.
// El patrón es siempre el mismo:
//   1. Intentar cargar el recurso desde archivo
//   2. Si falla, mostrar advertencia pero NO crashear
//   3. Si tiene éxito, almacenar en el mapa correspondiente
//   4. Para acceder, buscar en el mapa y lanzar excepción si no existe
// ============================================================================

#include "GestorRecursos.h"

// ============================================================================
// TEXTURAS
// ============================================================================

// Carga una textura desde disco y la almacena en el mapa con un nombre clave.
// Si la textura ya existe con ese nombre, se sobreescribe silenciosamente.
bool GestorRecursos::cargarTextura(const std::string& name, const std::string& filepath) {
    sf::Image img;
    if (!img.loadFromFile(filepath)) {
        std::cerr << "[GestorRecursos] ADVERTENCIA: No se pudo cargar la imagen '"
                  << name << "' desde: " << filepath << std::endl;
        return false;
    }

    // Aplicar máscara personalizada con tolerancia para el ruido de fondo (DALL-E / JPEG)
    for (unsigned int x = 0; x < img.getSize().x; ++x) {
        for (unsigned int y = 0; y < img.getSize().y; ++y) {
            sf::Color c = img.getPixel(x, y);
            // Ruido blanco (fondo casi blanco)
            if (c.r > 220 && c.g > 220 && c.b > 220) {
                img.setPixel(x, y, sf::Color(255, 255, 255, 0));
            }
            // Ruido verde (aura verde del zombie)
            // El zombie tiene tonos oscuros, el ruido suele ser verde neón/brillante
            else if (name == "zombie" && c.g > 150 && c.r < 120 && c.b < 120) {
                img.setPixel(x, y, sf::Color(0, 255, 0, 0));
            }
        }
    }

    sf::Texture texture;
    if (!texture.loadFromImage(img)) {
        std::cerr << "[GestorRecursos] ADVERTENCIA: No se pudo crear textura desde imagen '"
                  << name << "' (" << filepath << ")" << std::endl;
        return false;
    }

    // Almacenar la textura en el mapa usando std::move para eficiencia
    // std::move transfiere la propiedad sin copiar los datos de la imagen
    textures[name] = std::move(texture);

    std::cout << "[GestorRecursos] Textura cargada: " << name << std::endl;
    return true;
}

// Retorna una referencia a la textura solicitada.
// Usamos referencia (&) para evitar copiar la textura (que es costoso).
sf::Texture& GestorRecursos::getTextura(const std::string& name) {
    // Buscar la textura en el mapa
    auto it = textures.find(name);

    // Si no se encontró, lanzar una excepción descriptiva
    if (it == textures.end()) {
        throw std::runtime_error(
            "[GestorRecursos] ERROR: Textura no encontrada: '" + name + "'"
        );
    }

    // Retornar referencia al valor del par (clave, valor) del iterador
    return it->second;
}

// Verifica si una textura ya fue cargada previamente.
// Método const porque no modifica el estado del objeto.
bool GestorRecursos::tieneTextura(const std::string& name) const {
    // std::map::count() retorna 1 si existe la clave, 0 si no
    return textures.count(name) > 0;
}

// ============================================================================
// FUENTES TIPOGRÁFICAS
// ============================================================================

// Carga una fuente desde un archivo .ttf o .otf.
// Las fuentes son necesarias para renderizar texto en SFML.
bool GestorRecursos::cargarFuente(const std::string& name, const std::string& filepath) {
    sf::Font font;

    if (!font.loadFromFile(filepath)) {
        std::cerr << "[GestorRecursos] ADVERTENCIA: No se pudo cargar la fuente '"
                  << name << "' desde: " << filepath << std::endl;
        return false;
    }

    // Almacenar la fuente en el mapa
    fonts[name] = std::move(font);

    std::cout << "[GestorRecursos] Fuente cargada: " << name << std::endl;
    return true;
}

// Retorna referencia a la fuente solicitada.
sf::Font& GestorRecursos::getFuente(const std::string& name) {
    auto it = fonts.find(name);

    if (it == fonts.end()) {
        throw std::runtime_error(
            "[GestorRecursos] ERROR: Fuente no encontrada: '" + name + "'"
        );
    }

    return it->second;
}

// Verifica si una fuente ya está cargada.
bool GestorRecursos::tieneFuente(const std::string& name) const {
    return fonts.count(name) > 0;
}

// ============================================================================
// BUFFERS DE SONIDO
// ============================================================================

// Carga un buffer de sonido desde un archivo .wav o .ogg.
// Un SoundBuffer almacena los datos de audio en memoria.
// Luego se puede asignar a un sf::Sound para reproducirlo.
bool GestorRecursos::cargarBufferSonido(const std::string& name, const std::string& filepath) {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filepath)) {
        std::cerr << "[GestorRecursos] ADVERTENCIA: No se pudo cargar el sonido '"
                  << name << "' desde: " << filepath << std::endl;
        return false;
    }

    // Almacenar el buffer de sonido en el mapa
    soundBuffers[name] = std::move(buffer);

    std::cout << "[GestorRecursos] Sonido cargado: " << name << std::endl;
    return true;
}

// Retorna referencia al buffer de sonido solicitado.
sf::SoundBuffer& GestorRecursos::getBufferSonido(const std::string& name) {
    auto it = soundBuffers.find(name);

    if (it == soundBuffers.end()) {
        throw std::runtime_error(
            "[GestorRecursos] ERROR: Sonido no encontrado: '" + name + "'"
        );
    }

    return it->second;
}

// Verifica si un buffer de sonido ya está cargado.
bool GestorRecursos::tieneBufferSonido(const std::string& name) const {
    return soundBuffers.count(name) > 0;
}
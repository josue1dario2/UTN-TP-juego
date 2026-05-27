cmake_minimum_required(VERSION 3.19)
project(ZWave LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Descargar SFML 2.6.2 automáticamente ---
# FetchContent descarga y compila SFML sin necesidad de instalarla.
# La primera compilación tarda más, pero luego se cachea.
include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.2
    GIT_SHALLOW    TRUE    # Solo descarga el último commit (más rápido)
)
FetchContent_MakeAvailable(SFML)

# --- Archivos fuente del juego ---
set(SOURCES
    src/main.cpp
    src/Juego.cpp
    src/Entidad.cpp
    src/Jugador.cpp
    src/Zombie.cpp
    src/Arma.cpp
    src/ArmaDatos.cpp
    src/Bala.cpp
    src/Oleada.cpp
    src/HUD.cpp
    src/Menu.cpp
    src/GestorDeArchivos.cpp
    src/GestorRecursos.cpp
    src/Mapa.cpp
    src/Personaje.cpp
    src/Estadistica.cpp
    src/Partida.cpp
)

# --- Crear el ejecutable ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- Linkear las librerías de SFML ---
# sfml-graphics: para dibujar sprites, formas, texto
# sfml-window:   para manejar ventanas y eventos (teclado, mouse)
# sfml-system:   para utilidades (reloj, vectores)
# sfml-audio:    para música y efectos de sonido
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# --- Copiar assets al directorio de compilación ---
# Esto asegura que el ejecutable encuentre las texturas, sonidos, etc.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
    COMMENT "Copiando assets y datos al directorio de build..."
)

# --- Tool: Generador de Sprites ---
add_executable(gen_assets_tools gen_assets.cpp)
target_link_libraries(gen_assets_tools PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
add_custom_command(TARGET gen_assets_tools POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/textures
        $<TARGET_FILE_DIR:gen_assets_tools>/textures
    COMMENT "Copiando texturas para el generador..."
)

# --- Tool: Generador de Audio 8-bit ---
add_executable(audio_generator generador_audio.cpp)
# No necesita SFML, es C++ puro

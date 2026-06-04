cmake_minimum_required(VERSION 3.19)
project(TP_Juego LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SFML)

set(SOURCES
    src/main.cpp
    src/Juego.cpp
    src/Personaje.cpp
    src/ObjetoGrafico.cpp
    src/Entidad.cpp
    src/ObjetoMapa.cpp
    src/Oleada.cpp
    src/Zombie.cpp
    src/Arma.cpp
    src/Proyectil.cpp
    src/Puntero.cpp
    src/Menu.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} "include/Menu.h" "src/Menu.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()
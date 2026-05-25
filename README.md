# UTN-TP-juego

# Estructura
En assets va todo los graficos y audio (mp3, png, etc)
En build van los archivos temporales de compilacion y los .exe
En include van todos los .h
En src todo el codigo fuente
Gitignore es para no subir archivos basura de la compilacion

# compilar
g++ src/main.cpp src/Juego.cpp src/Personaje.cpp src/ObjetoGrafico.cpp src/Entidad.cpp -Iinclude -o build/juego -lsfml-graphics -lsfml-window -lsfml-system

# ejecuta
.\build\juego.exe

### Instalación de SFML

Abrir la terminal `MSYS2 UCRT64` y ejecutar:

```bash
pacman -S mingw-w64-ucrt-x86_64-sfml
```
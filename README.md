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

Las estructura de la herencia es la siguiente
ObjetoGrafico
│
├── Entidad
│   ├── Jugador
│   └── Enemigo (FALTA)
│
├── ObjetoMapa (FALTA)
│
└── Proyectil  (FALTA)

<img width="185" height="195" alt="image" src="https://github.com/user-attachments/assets/fbd6c543-6dae-4d0d-8443-bf9800e315be" />


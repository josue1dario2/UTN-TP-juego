# UTN-TP-juego

# Estructura
En assets va todo los graficos y audio (mp3, png, etc)
En build van los archivos temporales de compilacion y los .exe
En include van todos los .h
En src todo el codigo fuente
Gitignore es para no subir archivos basura de la compilacion

# compilar
g++ src/main.cpp src/Juego.cpp src/Personaje.cpp src/ObjetoGrafico.cpp src/Entidad.cpp src/ObjetoMapa.cpp -Iinclude -o build/juego -lsfml-graphics -lsfml-window -lsfml-system

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
├── ObjetoMapa
│
└── Proyectil  (FALTA)

## ObjetoGrafico
Se encarga de todas las funciones relacionadas a la posicion, a las texturas/sprites y la hitbox, las clases que la hereden tienen que hacer override a la funcion "actualizar" si quieren tener su propia logica

## Entidad
Hereda de [ObjetoGrafico](#objetografico), se encarga de las caracteristicas tipicas de cualquier objeto con "vida" dentro del juego

## Personaje
Hereda de [Entidad](#entidad), tiene toda la logica del jugador principal

## ObjetoMapa
Hereda de [ObjetoGrafico](#objetografico), se usa para elementos decorativos o que bloquean el paso a entidades.

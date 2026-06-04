# TC1030 - Tarea 2: Herencia y Polimorfismo

## Instrucciones para compilar y ejecutar

### Requisitos
- `g++` con soporte para C++17 o superior
- CMake 3.20+ (si se usa CLion)

### Compilación desde línea de comandos

```bash
g++ src/main.cpp src/dice.cpp src/player.cpp src/tile.cpp \
    src/normal_tile.cpp src/snake_tile.cpp src/ladder_tile.cpp \
    -o snakes.out
```

### Ejecución con valores por defecto (equivalente a Tarea 1)

```bash
./snakes.out
```

### Ejecución con parámetros personalizados

```bash
./snakes.out <tiles> <snakes> <ladders> <penalty> <reward> <players> <turns> <type>
```

| Parámetro | Descripción | Valor por defecto |
|-----------|-------------|-------------------|
| `tiles`   | Número de casillas del tablero | 30 |
| `snakes`  | Número de serpientes | 3 |
| `ladders` | Número de escaleras | 3 |
| `penalty` | Casillas que retrocede una serpiente | 3 |
| `reward`  | Casillas que avanza una escalera | 3 |
| `players` | Número de jugadores | 2 |
| `turns`   | Número máximo de turnos | 100 |
| `type`    | `M` = manual, `A` = automático | M |

### Ejemplos

```bash
# Manual con configuración por defecto
./snakes.out 30 3 3 3 3 2 100 M

# Automático, tablero grande
./snakes.out 50 5 5 4 4 3 200 A

# Con archivo de entrada (modo manual)
./snakes.out < myInput.txt > myOutput.txt
```

### Archivo de entrada de ejemplo (`myInput.txt`)

```
C
C
C
C
C
E
```

---

## Formato de salida por turno

```
<turno> <jugador> <posición_actual> <dado> <tipo_casilla> <posición_final>
```

### Tipos de casilla

| Letra | Tipo | Efecto |
|-------|------|--------|
| `N` | Normal | Sin efecto |
| `S` | Serpiente | Retrocede `penalty` casillas |
| `L` | Escalera | Avanza `reward` casillas adicionales |

### Ejemplo de salida

```
Press C to continue next turn, or E to end the game:
1 1 1 4 N 5
2 2 1 5 N 6
3 1 5 3 L 11
4 2 6 6 S 9
-- GAME OVER --
Player 1 is the winner!!!
```

---

## Estructura del proyecto

```
SnakesAndLadders/
├── CMakeLists.txt
├── README.md
└── src/
    ├── main.cpp          <- main(), MyGame, ManualGame, AutoGame
    ├── tile.h / tile.cpp
    ├── normal_tile.h / normal_tile.cpp
    ├── snake_tile.h / snake_tile.cpp
    ├── ladder_tile.h / ladder_tile.cpp
    ├── player.h / player.cpp
    ├── dice.h / dice.cpp
```

---

## Diseño orientado a objetos

### Jerarquía de casillas — polimorfismo

`Tile` es la clase base abstracta que define la interfaz común de todas las casillas. Declara el método virtual puro `applyEffect(pos)`, que cada subclase implementa según su comportamiento:

- `NormalTile` — devuelve la posición sin modificar.
- `SnakeTile` — resta `penalty` a la posición, con clamp a 1 como mínimo.
- `LadderTile` — suma `reward` a la posición, con clamp al tamaño del tablero como máximo.

`MyGame` mantiene un arreglo de punteros `Tile**`, lo que permite invocar `tiles[i]->applyEffect(pos)` sin conocer el subtipo concreto en tiempo de compilación. El tipo correcto se resuelve en tiempo de ejecución gracias al mecanismo de vtable de C++.

### Jerarquía de juego — herencia

`MyGame` es la clase base abstracta que encapsula el estado común del juego (tablero, jugadores, dado, contadores) y declara `start()` y `runTurn()` como métodos virtuales puros.

- `ManualGame` — implementa `start()` leyendo input del usuario (`C`/`E`) entre cada turno, siguiendo la dinámica interactiva de la Tarea 1.
- `AutoGame` — implementa `start()` ejecutando todos los turnos de forma automática sin requerir input, hasta que un jugador gane o se alcance el límite de turnos.

El `main()` recibe el tipo de juego por argumento y crea la instancia correcta mediante un puntero a la clase base (`MyGame*`), aprovechando el polimorfismo para llamar al `start()` adecuado sin condicionales adicionales.

---

## Conceptos de POO aplicados

### Herencia
Dos jerarquías independientes: `Tile → {NormalTile, SnakeTile, LadderTile}` y `MyGame → {ManualGame, AutoGame}`. Las subclases reutilizan el estado y los métodos comunes de la base, sobreescribiendo únicamente el comportamiento que las diferencia.

### Polimorfismo en tiempo de ejecución
El arreglo `Tile** tiles` almacena punteros a la clase base. La llamada `tiles[i]->applyEffect(pos)` se resuelve dinámicamente según el tipo real del objeto apuntado. Lo mismo ocurre con `game->start()` desde `main()`: se llama al método correcto de `ManualGame` o `AutoGame` sin que el código cliente sepa cuál es.

### Encapsulamiento
El estado del juego (tablero, jugadores, contadores) es privado o protegido en `MyGame`. Las subclases acceden a él mediante los atributos protegidos declarados en la base, sin exponer la representación interna al exterior.

### Abstracción
`MyGame` define el contrato del juego (qué debe hacer) sin implementar los detalles (cómo lo hace). `Tile` define el contrato de una casilla con `applyEffect()` como única interfaz pública relevante.

### Responsabilidad única (SRP)
- `Dice` — simula el dado.
- `Player` — gestiona identidad y posición.
- `Tile` y subclases — modelan el efecto de cada casilla.
- `MyGame` y subclases — coordinan el flujo del juego.

---

## Decisiones de diseño

- Las posiciones de serpientes y escaleras se generan **aleatoriamente** en cada partida usando `rand()`, evitando las casillas 1 y última para garantizar jugabilidad.
- Si dos casillas especiales coincidirían en la misma posición durante la construcción, el algoritmo reintenta hasta encontrar una casilla normal disponible.
- Solo se aplica **un efecto por turno**: si el resultado de una serpiente o escalera cae en otra casilla especial, esta segunda se ignora (sección 3.3.7.4 del enunciado).
- El destructor de `MyGame` libera correctamente la memoria dinámica del arreglo polimórfico (`Tile**`) y del arreglo de jugadores.
- El `main()` usa un puntero `MyGame*` para instanciar `ManualGame` o `AutoGame` según el argumento recibido, lo que demuestra polimorfismo de forma directa y limpia.

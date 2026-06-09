# TC1030 - Tarea 3: Sobrecarga de Operadores

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

### Ejecución con valores por defecto

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

# Automático
./snakes.out 30 3 3 3 3 2 100 A

# Con archivo de entrada/salida
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
| `S` | Serpiente | Retrocede `penalty` casillas (siempre resta) |
| `L` | Escalera | Avanza `reward` casillas adicionales |

### Ejemplo de salida

```
Press C to continue next turn, or E to end the game:
1 1 1 4 N 5
2 2 1 5 N 6
3 1 5 3 L 11
4 2 6 6 S 8
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
    ├── main.cpp           <- main(), Turn, MyGame, ManualGame, AutoGame
    ├── tile.h / tile.cpp
    ├── normal_tile.h / normal_tile.cpp
    ├── snake_tile.h / snake_tile.cpp
    ├── ladder_tile.h / ladder_tile.cpp
    ├── player.h / player.cpp
    ├── dice.h / dice.cpp
```

---

## Diseño orientado a objetos

### Jerarquía de casillas — `operator+`

`Tile` es la clase base abstracta que ahora declara `operator+` como método virtual puro en lugar de `applyEffect`. Cada subclase lo implementa según su tipo:

- `NormalTile::operator+(pos)` — devuelve `pos` sin modificación.
- `SnakeTile::operator+(pos)` — devuelve `pos - abs(penalty)`, garantizando que siempre se reste independientemente del signo del valor almacenado. El resultado se hace clamp a 1 como mínimo.
- `LadderTile::operator+(pos)` — devuelve `pos + reward` con clamp al tamaño del tablero como máximo.

Además, `Tile` declara una función libre `friend int operator+(int pos, const Tile& t)` que delega en el método virtual. Esto permite la sintaxis natural `landed + *tiles[i]`, donde el entero queda a la izquierda y la casilla a la derecha, y el polimorfismo resuelve en tiempo de ejecución qué implementación ejecutar.

### Clase `Turn` — `operator<<`

`Turn` es una clase (implementada como `struct`) que agrupa los seis datos de cada turno: número de turno, identificador del jugador, posición previa, valor del dado, tipo de casilla y posición final. Sobreescribe `operator<<` como función `friend` de `std::ostream`, lo que permite imprimir el turno directamente con `std::cout << t`.

Esto reemplaza la llamada manual a `printTurn(...)` de la Tarea 2. La lógica de formato queda encapsulada dentro de `Turn`, siguiendo el principio de responsabilidad única.

---

## Sobrecarga de operadores

### `operator+` en la jerarquía `Tile`

**Motivación:** Las casillas tienen efectos distintos sobre la posición del jugador. En lugar de delegar ese cálculo a un método genérico `applyEffect`, se sobrecarga `operator+` para que cada tipo de casilla exprese su comportamiento de forma semántica: sumar una posición a una casilla produce la nueva posición del jugador.

**Implementación:**

```cpp
// Función libre en tile.h — permite: int + Tile
friend int operator+(int pos, const Tile& t) {
    return t.operator+(pos);
}

// NormalTile — sin efecto
int NormalTile::operator+(int pos) const {
    return pos;
}

// SnakeTile — siempre resta el valor absoluto del penalty
int SnakeTile::operator+(int pos) const {
    int result = pos - std::abs(penalty);
    return (result < 1) ? 1 : result;
}

// LadderTile — suma el reward con clamp al tamaño del tablero
int LadderTile::operator+(int pos) const {
    int result = pos + reward;
    return (result > size) ? size : result;
}
```

**Uso en el turno:**

```cpp
// antes (Tarea 2)
int finalPos = tiles[landed - 1]->applyEffect(landed);

// ahora (Tarea 3)
int finalPos = landed + *tiles[landed - 1];
```

El polimorfismo en tiempo de ejecución garantiza que se llame al `operator+` correcto según el tipo real de la casilla, sin ningún condicional en el código del turno.

**Ejemplo del enunciado:**
Jugador en casilla 5, saca 4 en el dado → aterriza en casilla 9. La casilla 9 es una serpiente con `penalty = 4`. El operador calcula `9 - abs(4) = 5`. Si el penalty fuera 8, calcularía `9 - 8 = 1` (clamp a 1 mínimo).

### `operator<<` en `Turn`

**Motivación:** Encapsular el formato de salida del turno dentro de la propia clase, en lugar de dispersar la lógica de impresión en `ManualGame` y `AutoGame` por separado.

**Implementación:**

```cpp
friend std::ostream& operator<<(std::ostream& os, const Turn& t) {
    os << t.turnNum  << " "
       << t.playerId << " "
       << t.prevPos  << " "
       << t.roll     << " "
       << t.tileType << " "
       << t.finalPos;
    return os;
}
```

**Uso:**

```cpp
Turn t { turnCount, p.getId(), prevPos, roll,
         tiles[landed - 1]->getType(), finalPos };
std::cout << t << std::endl;
```

Devolver `os` por referencia permite encadenar operaciones como `std::cout << t << std::endl`, siguiendo la convención estándar de C++.

---

## Conceptos de POO aplicados

### Herencia y polimorfismo (continuación de Tarea 2)
La jerarquía `Tile → {NormalTile, SnakeTile, LadderTile}` y `MyGame → {ManualGame, AutoGame}` se mantiene intacta. El `operator+` virtual puro reemplaza a `applyEffect` como punto de extensión polimórfico.

### Sobrecarga de operadores
Se sobrecargan dos operadores distintos con propósitos complementarios: `operator+` para encapsular la lógica de efectos de casilla con sintaxis expresiva, y `operator<<` para centralizar el formato de salida del turno.

### Encapsulamiento
`Turn` encapsula el formato de impresión. Las subclases de `Tile` encapsulan sus propias reglas de movimiento. Ninguna clase exterior necesita conocer los detalles de cálculo o formato.

### Responsabilidad única (SRP)
- `Turn` — sabe cómo imprimirse.
- `Tile` y subclases — saben cómo mover al jugador.
- `MyGame` y subclases — coordinan el flujo del juego.

---

## Decisiones de diseño

- `operator+` se declara como método virtual en `Tile` y se expone también como función libre `friend`, lo que habilita la sintaxis `int + Tile` sin romper el polimorfismo.
- `SnakeTile::operator+` usa `std::abs(penalty)` para garantizar que siempre se reste, independientemente de cómo se haya inicializado el valor de penalización.
- `Turn` se implementa como `struct` porque todos sus miembros son datos planos de salida; no requiere encapsulamiento adicional.
- La salida en consola es idéntica a las Tareas 1 y 2. Los únicos cambios son internos al diseño.

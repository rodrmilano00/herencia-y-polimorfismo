# TC1030 - Serpientes y Escaleras

## Instrucciones para compilar y ejecutar

### Requisitos
- `g++` 
- CMake (si se usa CLion)

### Compilación desde línea de comandos

```bash
g++ src/main.cpp src/dice.cpp src/board.cpp src/player.cpp -o snakes.out
```
### Compilación desde CLion
- Desde GUI

### Ejecución

```bash
./snakes.out
```



---

## Controles del juego

| Tecla | Acción |
|-------|--------|
| `C`   | Continuar al siguiente turno |
| `E`   | Terminar el juego |

---

## Formato de salida por turno

Cada turno imprime en consola una línea con el siguiente formato:

```
<turno> <jugador> <posición_actual> <dado> <tipo_casilla> <posición_final>
```

### Ejemplo

```
Turn: 10 Player: 2 Previous position: 13 Dice: 5 Tile type: N Final position: 18
Turn: 11 Player: 1 Previous position: 24 Dice: 6 Tile type: N Final position: 30
```

---

## Estructura del proyecto

```
Dise-oOrientadoObjetos/
├── CMakeLists.txt
└── src/
    ├── snakes.cpp      <- main() y clase MyGame
    ├── dice.h
    ├── dice.cpp
    ├── board.h
    ├── board.cpp
    ├── player.h
    └── player.cpp
```

---

## Diseño orientado a objetos

### Clases implementadas

#### `Dice`
Encapsula la simulación de un dado de 6 caras. Inicializa la semilla aleatoria en el constructor con `time(nullptr)` para garantizar resultados distintos en cada ejecución. Expone únicamente el método `roll()` para obtener un valor entre 1 y 6.

#### `Player`
Modelo simple que representa a un jugador. Almacena su identificador (`id`) y su posición actual en el tablero (`position`), inicializada en 1. Expone getters y un setter para la posición.

#### `Board`
Encapsula el tablero de 30 casillas. Las posiciones de serpientes y escaleras se definen de forma fija en el método privado `placeTiles()`. El método `applyEffect(pos)` calcula la posición final después de aplicar el efecto de la casilla (penalización de -3 para serpientes, recompensa de +3 para escaleras), respetando los límites del tablero.

#### `MyGame`
Clase central exigida por el enunciado. Orquesta el flujo completo del juego a través del método público `start()`. Posee (por composición) un `Board`, dos `Player` y un `Dice`. El resto de métodos son privados para encapsular la lógica interna.

---

## Conceptos de POO aplicados

### Encapsulamiento
Cada clase expone únicamente la interfaz necesaria. `MyGame` tiene un solo método público (`start()`); toda la lógica del turno, lectura de input y verificación de ganador se mantiene privada. `Board` oculta completamente la representación interna del arreglo de casillas.

### Responsabilidad única (S de SOLID)
- `Dice` solo sabe tirar el dado.
- `Player` solo gestiona su identidad y posición.
- `Board` solo conoce la distribución y efectos del tablero.
- `MyGame` solo coordina el flujo del juego.

### Composición
`MyGame` contiene a `Board`, `Player[2]` y `Dice` como atributos de valor directo. Se eligió composición sobre herencia porque la relación semántica es de pertenencia (*tiene-un*), no de especialización (*es-un*). Los objetos no tienen sentido ni ciclo de vida fuera del juego.

### Abstracción
El método `applyEffect()` en `Board` abstrae la lógica de efectos especiales. Desde `MyGame` no es necesario conocer los valores de penalización o recompensa; simplemente se consulta el efecto de una posición y se obtiene la posición final.

---

## Decisiones de diseño

- Las posiciones de serpientes y escaleras son **fijas** (hardcoded en `Board`) para simplificar la implementación y garantizar reproducibilidad en las pruebas.
- El límite máximo de turnos es **100** para evitar partidas infinitas.
- Cuando un jugador cae en una casilla especial cuyo efecto lo llevaría fuera del tablero, se aplica un **clamp** al rango `[1, 30]`.
- Solo se permite **un efecto por turno**: si el resultado de una serpiente o escalera cae en otra casilla especial, esta segunda se ignora (conforme al enunciado, sección 3.3.7.4).

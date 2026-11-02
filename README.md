# 2nd_Practica_ADA

Segunda práctica de Análisis y Diseño de Algoritmos (ADA). Implementa la multiplicación de enteros grandes representados como cadenas de dígitos, comparando la multiplicación clásica Θ(n²) con el algoritmo de Karatsuba (divide y vencerás).

## Integrantes

- Simón Mazo Gomez
- Mateo Gómez Giraldo
- Johan Alejandro Peña Rivera

## Estructura del código (`src/`)

- `classic.cpp` / `classic.h`: multiplicación clásica (`multiplicacionClasica`).
- `bigint_ops.cpp` / `bigint_ops.h`: operaciones auxiliares sobre cadenas (`sumar`, `restar`, `desplazar`, `partir`).
- `karatsuba.cpp` / `karatsuba.h`: algoritmo de Karatsuba.
- `main.cpp`: programa de ejemplo que multiplica dos números con Karatsuba.
- `tests.cpp`: pruebas de correctitud.
- `bench.cpp`: mediciones de tiempo (genera `data/tiempos.csv`).

Otras carpetas: `data/` (tiempos medidos) y `docs/` (informe y gráfica).

## Instrucciones de compilación

Requisitos: un compilador de C++ con soporte para C++17 (`g++` o `clang++`).

Desde la raíz del repositorio:

```bash
# Programa de ejemplo
g++ -std=c++17 -O2 -Wall -Wextra -o main src/main.cpp src/classic.cpp src/karatsuba.cpp src/bigint_ops.cpp

# Pruebas de correctitud
g++ -std=c++17 -O2 -Wall -Wextra -o tests src/tests.cpp src/classic.cpp src/karatsuba.cpp src/bigint_ops.cpp

# Mediciones de tiempo
g++ -std=c++17 -O2 -Wall -Wextra -o bench src/bench.cpp src/classic.cpp src/karatsuba.cpp src/bigint_ops.cpp
```

Cada ejecutable tiene su propio `main`, por lo que se compila por separado, siempre junto con `classic.cpp`, `karatsuba.cpp` y `bigint_ops.cpp`; de lo contrario el enlazador mostrará errores de `Undefined symbols`.

Se recomienda `-O2`: sin optimización los tiempos no son representativos.

## Ejecución

- Programa de ejemplo: `./main`
- Pruebas de correctitud: `./tests` (con `./tests -v` muestra cada caso).
- Mediciones: `./bench data/tiempos.csv` (la carpeta `data/` debe existir).

En Windows, los ejecutables son `main.exe`, `tests.exe` y `bench.exe`.

## Representación de los números

Cada número es un `std::string` de dígitos en base 10, con el dígito más significativo primero.

`partir(a, m)` separa `a` en dos partes y devuelve `{alta, baja}`: `baja` son los últimos `m` dígitos y `alta` los restantes (`"0"` si `a` tiene `m` dígitos o menos).

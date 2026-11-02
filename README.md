# Práctica 2 — Karatsuba (ADA)

Implementación en C++ de la multiplicación clásica Θ(n²) y del algoritmo de Karatsuba,
con pruebas de correctitud y mediciones de tiempo.


## Compilar

Desde la raíz del repositorio (requiere `g++` con C++17; no hay Makefile ni CMake):

```
g++ -std=c++17 -O2 -Wall -Wextra -o tests src/tests.cpp src/classic.cpp src/karatsuba.cpp src/bigint_ops.cpp
g++ -std=c++17 -O2 -Wall -Wextra -o bench src/bench.cpp src/classic.cpp src/karatsuba.cpp src/bigint_ops.cpp
g++ -std=c++17 -O2 -Wall -Wextra -o main src/main.cpp src/classic.cpp src/karatsuba.cpp src/bigint_ops.cpp
```

Cada comando genera un ejecutable distinto:

- `tests`: pruebas de correctitud.
- `bench`: mediciones de tiempo.
- `main`: ejemplo simple de Karatsuba, con valores fijos en `src/main.cpp`.

`tests.cpp`, `bench.cpp` y `main.cpp` definen cada uno su propia función `main()`, así que cada
ejecutable se compila con uno solo de ellos.

Se recomienda `-O2`: sin optimización los tiempos no son representativos.

## Ejecutar

- Pruebas de correctitud: `./tests` (con `./tests -v` muestra cada caso).
- Mediciones: `./bench data/tiempos.csv` (la carpeta `data/` debe existir). Sobrescribe el archivo
  indicado; sin argumentos usa `data/tiempos.csv`.
- Ejemplo: `./main`.

En Windows, los ejecutables son `tests.exe`, `bench.exe` y `main.exe`.

## Representación de los números

Cada número es un `std::string` de dígitos en base 10, con el dígito más significativo primero.
`partir(a, m)`: [A: completar — significado de `m` y orden de los valores devueltos].

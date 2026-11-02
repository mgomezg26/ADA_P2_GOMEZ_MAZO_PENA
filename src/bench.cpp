// bench.cpp -- Mediciones de tiempo: multiplicación clásica vs Karatsuba 

//
// Protocolo de medición:
//  - Reloj: std::chrono::steady_clock (monótono, no lo afectan cambios de hora del sistema).
//  - Tamaños: n = 2, 4, 8, ..., 4096 dígitos (potencias de 2); dos números aleatorios de n dígitos.
//  - Repeticiones: cada medición repite el cálculo hasta acumular al menos T_MIN segundos (y R_MIN
//    repeticiones), de modo que el tiempo total supere con holgura la resolución del reloj;
//    luego se divide entre el número de repeticiones.
//  - Se rota entre N_PARES pares de entradas distintos para no medir un solo caso.

#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "classic.h"
#include "karatsuba.h"

using namespace std;
using clk = chrono::steady_clock;

static const double T_MIN    = 0.10;   // segundos mínimos acumulados por pasada (>> resolución del reloj)
static const int    R_MIN    = 5;      // repeticiones mínimas por pasada
static const int    N_PARES  = 4;      // pares de entradas distintos por tamaño
static const int    N_PASADAS = 3;     // se toma el mínimo de estas pasadas
static const size_t N_MIN = 2, N_MAX = 4096;

static volatile unsigned long long sumidero = 0;   // consume los resultados

static string aleatorio(mt19937& g, size_t n) {
    string s(n, '0');
    for (auto& c : s) c = '0' + g() % 10;
    if (s[0] == '0') s[0] = '1' + g() % 9;
    return s;
}

static unsigned long long checksum(const string& s) {   // FNV-1a sobre los dígitos
    unsigned long long h = 1469598103934665603ULL;
    for (char c : s) { h ^= (unsigned char)c; h *= 1099511628211ULL; }
    return h;
}

struct Medida { double us; long reps; unsigned long long chk; };

template <class F>
static Medida una_pasada(F f, const vector<string>& A, const vector<string>& B) {
    unsigned long long chk = 0;
    long reps = 0;
    auto t0 = clk::now();
    double el = 0;
    do {
        for (int i = 0; i < N_PARES; ++i) {
            string r = f(A[i], B[i]);
            chk += checksum(r);
            ++reps;
        }
        el = chrono::duration<double>(clk::now() - t0).count();
    } while (el < T_MIN || reps < R_MIN);
    return {el / reps * 1e6, reps, chk};   // microsegundos por multiplicación
}

template <class F>
static Medida medir(F f, const vector<string>& A, const vector<string>& B) {
    f(A[0], B[0]);   // calentamiento (cachés, asignador de memoria)
    Medida mejor = una_pasada(f, A, B);
    for (int t = 1; t < N_PASADAS; ++t) {
        Medida m = una_pasada(f, A, B);
        if (m.us < mejor.us) mejor = m;
    }
    sumidero = sumidero + mejor.chk;
    return mejor;
}

int main(int argc, char** argv) {
    string salida = (argc > 1) ? argv[1] : "data/tiempos.csv";
    ofstream csv(salida);
    if (!csv) { cerr << "No se pudo abrir " << salida << " (¿existe la carpeta data/?)\n"; return 1; }

    mt19937 g(2024);   // semilla fija: mismas entradas en cada corrida
    csv << "n,clasica_us,karatsuba_us,razon_clasica_sobre_karatsuba,reps_clasica,reps_karatsuba,coinciden\n";
    printf("%6s %15s %15s %10s\n", "n", "clasica (us)", "karatsuba (us)", "cl/kar");

    int distintos = 0;
    size_t cruce = 0;   // primer n donde Karatsuba es más rápido que la clásica (0 = no aparece)
    for (size_t n = N_MIN; n <= N_MAX; n *= 2) {
        vector<string> A, B;
        for (int i = 0; i < N_PARES; ++i) { A.push_back(aleatorio(g, n)); B.push_back(aleatorio(g, n)); }

        bool ok = true;   // verificación: ambos algoritmos deben dar el mismo producto
        for (int i = 0; i < N_PARES; ++i) ok = ok && (multiplicacionClasica(A[i], B[i]) == karatsuba(A[i], B[i]));
        if (!ok) ++distintos;

        Medida c = medir(multiplicacionClasica, A, B);
        Medida k = medir(karatsuba, A, B);
        double razon = c.us / k.us;   // > 1  => Karatsuba más rápido
        if (cruce == 0 && razon > 1.0) cruce = n;

        csv << n << "," << c.us << "," << k.us << "," << razon << "," << c.reps << "," << k.reps << "," << (ok ? 1 : 0) << "\n";
        printf("%6zu %15.3f %15.3f %10.3f%s\n", n, c.us, k.us, razon, ok ? "" : "   <-- RESULTADOS DISTINTOS");
        fflush(stdout);
    }

    cout << "\nPunto de cruce (Karatsuba mas rapido que la clasica): ";
    if (cruce) cout << "n = " << cruce << " digitos\n";
    else cout << "no aparece en el rango " << N_MIN << ".." << N_MAX << "\n";
    cout << "Verificacion de resultados: " << (distintos ? "HAY DIFERENCIAS" : "todos coinciden") << "\n";
    cout << "(checksum consumido: " << sumidero << ")\n";
    cout << "Tabla escrita en " << salida << "\n";
    return distintos ? 1 : 0;
}
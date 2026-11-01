// tests.cpp -- Validación de correctitud
// Compara karatsuba(A,B) contra multiplicacionClasica(A,B) y contra valores esperados conocidos.

#include <iostream>
#include <random>
#include <string>
#include "classic.h"
#include "karatsuba.h"

using namespace std;

static int total = 0, fallos = 0;

static void check(const string& nombre, bool ok, const string& detalle = "") {
    ++total;
    if (!ok) { ++fallos; cout << "  [FALLA] " << nombre << " " << detalle << "\n"; }
}
// Karatsuba debe coincidir con la clásica.
static void igual(const string& nombre, const string& a, const string& b) {
    string c = multiplicacionClasica(a, b), k = karatsuba(a, b);
    check(nombre, c == k, "\n     A=" + a.substr(0, 30) + "\n     B=" + b.substr(0, 30) +
          "\n     clasica=" + c.substr(0, 40) + "\n     karatsuba=" + k.substr(0, 40));
}
// Ambos algoritmos deben dar un valor esperado conocido (verificación independiente).
static void esperado(const string& nombre, const string& a, const string& b, const string& e) {
    string c = multiplicacionClasica(a, b), k = karatsuba(a, b);
    check(nombre + " (clasica)", c == e, "got " + c.substr(0, 40));
    check(nombre + " (karatsuba)", k == e, "got " + k.substr(0, 40));
}
static string aleatorio(mt19937& g, size_t len) {
    string s(len, '0');
    for (auto& ch : s) ch = '0' + g() % 10;
    if (len > 0 && s[0] == '0') s[0] = '1' + g() % 9;
    return s;
}

int main() {
    mt19937 g(12345);

    cout << "== 1. Valores esperados conocidos ==\n";
    esperado("1234*5678", "1234", "5678", "7006652");
    esperado("23*45", "23", "45", "1035");
    esperado("99*99", "99", "99", "9801");
    esperado("0*0", "0", "0", "0");
    esperado("0*123", "0", "123", "0");
    esperado("1*987654321", "1", "987654321", "987654321");
    esperado("10^3*10^4", "1000", "10000", "10000000");

    cout << "== 2a. Numeros de 1 digito (las 100 combinaciones) ==\n";
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            esperado("1 digito " + to_string(i) + "*" + to_string(j),
                     string(1, '0' + i), string(1, '0' + j), to_string(i * j));

    cout << "== 2b. Muchos ceros a la izquierda ==\n";
    igual("000000123*0045", "000000123", "0045");
    esperado("0000*0000", "0000", "0000", "0");
    igual("00000000000000007*00000000000000009", "00000000000000007", "00000000000000009");
    igual("000999*999", "000999", "999");
    check("resultado sin ceros a la izquierda", karatsuba("0000012", "00003") == "36");

    cout << "== 2c. Factores con ceros intercalados ==\n";
    esperado("1001*1001", "1001", "1001", "1002001");
    igual("100200300*405006007", "100200300", "405006007");
    igual("10101010*90909090", "10101010", "90909090");
    igual("1000000007*1000000009", "1000000007", "1000000009");

    cout << "== 2d. Longitudes impares (antes del relleno) ==\n";
    for (size_t len : {1, 3, 5, 7, 9, 11, 13, 15, 17, 31, 33, 63, 65, 127, 129})
        igual("longitud impar " + to_string(len), aleatorio(g, len), aleatorio(g, len));
    igual("longitudes distintas 3 y 11", "123", "98765432101");
    igual("longitudes distintas 1 y 50", "7", aleatorio(g, 50));
    igual("longitudes distintas 40 y 5", aleatorio(g, 40), "12345");

    cout << "== 2e. Productos grandes (varios cientos de digitos) ==\n";
    igual("500x500", aleatorio(g, 500), aleatorio(g, 500));
    igual("1000x1000", aleatorio(g, 1000), aleatorio(g, 1000));
    igual("777x1234", aleatorio(g, 777), aleatorio(g, 1234));
    {   // verificación independiente: (10^n - 1)^2 = (n-1 nueves) 8 (n-1 ceros) 1
        size_t n = 600;
        string nueves(n, '9');
        esperado("(10^600-1)^2", nueves, nueves, string(n - 1, '9') + "8" + string(n - 1, '0') + "1");
        esperado("10^400*10^500", "1" + string(400, '0'), "1" + string(500, '0'), "1" + string(900, '0'));
    }

    cout << "\nResumen: " << (total - fallos) << "/" << total << " comprobaciones OK"
              << (fallos ? "  -> HAY FALLOS\n" : "  -> TODO CORRECTO\n");
    return fallos ? 1 : 0;
}
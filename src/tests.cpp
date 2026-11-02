// tests.cpp -- Validación de correctitud
// Compara karatsuba(A,B) contra multiplicacionClasica(A,B) y contra valores esperados conocidos.

#include <iostream>
#include <random>
#include <string>
#include "classic.h"
#include "karatsuba.h"
#include "bigint_ops.h"
 
using namespace std;
 
static int total = 0, fallos = 0;
static bool detallado = false;   
 
// Abrevia números largos: primeros 12 dígitos ... últimos 12 (n dígitos)
static string abrev(const string& s) {
    if (s.size() <= 30) return s;
    return s.substr(0, 12) + "..." + s.substr(s.size() - 12) + " (" + to_string(s.size()) + " digitos)";
}
 
static void check(const string& nombre, bool ok, const string& detalle = "") {
    ++total;
    if (!ok) { ++fallos; cout << "  [FALLA] " << nombre << " " << detalle << "\n"; }
}
// Karatsuba debe coincidir con la clásica.
static void igual(const string& nombre, const string& a, const string& b) {
    string c = multiplicacionClasica(a, b), k = karatsuba(a, b);
    bool ok = (c == k);
    check(nombre, ok, "\n     A=" + a.substr(0, 30) + "\n     B=" + b.substr(0, 30) +
          "\n     clasica=" + c.substr(0, 40) + "\n     karatsuba=" + k.substr(0, 40));
    if (detallado && ok)
        cout << "  OK  " << nombre << ":  " << abrev(a) << " x " << abrev(b) << " = " << abrev(k) << "\n";
}
// Ambos algoritmos deben dar un valor esperado conocido (verificación independiente).
static void esperado(const string& nombre, const string& a, const string& b, const string& e) {
    string c = multiplicacionClasica(a, b), k = karatsuba(a, b);
    check(nombre + " (clasica)", c == e, "got " + c.substr(0, 40));
    check(nombre + " (karatsuba)", k == e, "got " + k.substr(0, 40));
    if (detallado && c == e && k == e)
        cout << "  OK  " << nombre << ":  " << abrev(a) << " x " << abrev(b) << " = " << abrev(k) << "\n";
}
static string aleatorio(mt19937& g, size_t len) {
    string s(len, '0');
    for (auto& ch : s) ch = '0' + g() % 10;
    if (len > 0 && s[0] == '0') s[0] = '1' + g() % 9;
    return s;
}
 
int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) if (string(argv[i]) == "-v") detallado = true;
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
    if (detallado) cout << "  OK  0000012 x 00003 = " << karatsuba("0000012", "00003") << "\n";
 
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
 
    cout << "== 3. Casos extra ==\n";
    {   // 3a. Todo nueves: acarreos máximos y sumas de mitades con un dígito de más
        for (size_t n : {2, 3, 4, 7, 8, 16, 33, 64, 256})
            igual("todos 9s n=" + to_string(n), string(n, '9'), string(n, '9'));
        // 3b. Potencias de 10 y múltiplos
        igual("5*10^30 x 2*10^30", "5" + string(30, '0'), "2" + string(30, '0'));
        igual("10^64 x 10^64", "1" + string(64, '0'), "1" + string(64, '0'));
        // 3c. Patrones alternados 9090.. y 1010..
        string alt, alt2;
        for (int i = 0; i < 200; ++i) { alt += (i % 2 ? '0' : '9'); alt2 += (i % 2 ? '1' : '0'); }
        igual("alternado 9090.. x 0101..", alt, alt2);
        // 3d. Un factor enorme y el otro de 1 o 2 dígitos
        igual("1 digito x 300 digitos", "9", aleatorio(g, 300));
        igual("2 digitos x 300 digitos", "99", aleatorio(g, 300));
        // 3e. Mismo número (cuadrado) y números iguales con distinto relleno
        string q = aleatorio(g, 128);
        igual("cuadrado de 128 digitos", q, q);
        igual("relleno distinto 000q x q", "000" + q, q);
        // 3f. Longitud potencia de 2 exacta, y justo una más/menos
        for (size_t n : {32, 31, 33, 128, 127, 129})
            igual("n=" + to_string(n) + " aleatorio", aleatorio(g, n), aleatorio(g, n));
        // 3g. Propiedades algebraicas (no dependen de un valor esperado)
        string a = aleatorio(g, 300), b = aleatorio(g, 280), c = aleatorio(g, 150);
        check("conmutativa a*b = b*a", karatsuba(a, b) == karatsuba(b, a));
        check("x*1 = x", karatsuba(a, "1") == a);
        check("x*0 = 0", karatsuba(a, "0") == "0");
        check("distributiva a*(b+c) = a*b + a*c",
              karatsuba(a, sumar(b, c)) == sumar(karatsuba(a, b), karatsuba(a, c)));
        check("asociativa (a*b)*c = a*(b*c)",
              karatsuba(karatsuba(a, b), c) == karatsuba(a, karatsuba(b, c)));
        if (detallado) cout << "  OK  propiedades: conmutativa, x*1, x*0, distributiva, asociativa (a=300, b=280, c=150 digitos)\n";
        // 3h. Comprobaciones directas de las auxiliares de A que afectan a Karatsuba
        check("restar(1000,1)=999", restar("1000", "1") == "999");
        check("restar(500,500)=0", restar("500", "500") == "0");
        check("restar normaliza ceros", restar("1005", "1000") == "5");
        check("sumar 999+1=1000", sumar("999", "1") == "1000");
        check("sumar longitudes distintas", sumar("5", "99999") == "100004");
        check("desplazar(123,3)=123000", desplazar("123", 3) == "123000");
        auto pt = partir("123456", 3);
        check("partir(123456,3) = {123,456}", pt.first == "123" && pt.second == "456");
    }
 
    cout << "== 4. Prueba aleatoria masiva ==\n";
    {
        const int N_PEQ = 5000, N_GRA = 40;
        int malas = 0;
        for (int t = 0; t < N_PEQ; ++t) {
            size_t la = 1 + g() % 200, lb = 1 + g() % 200;
            string a = aleatorio(g, la), b = aleatorio(g, lb);
            if (g() % 4 == 0) a = string(1 + g() % 5, '0') + a;                  // 25%: ceros a la izquierda
            if (g() % 4 == 0) b = string(1 + g() % 5, '0') + b;
            if (g() % 5 == 0 && la > 4) for (size_t i = la / 3; i < 2 * la / 3; ++i) a[i] = '0';  // 20%: bloque de ceros
            if (g() % 5 == 0 && lb > 4) for (size_t i = lb / 3; i < 2 * lb / 3; ++i) b[i] = '9';  // 20%: bloque de nueves
            string c = multiplicacionClasica(a, b), k = karatsuba(a, b);
            if (c != k) {
                if (++malas <= 3) cout << "  [FALLA aleatoria] A=" << a << "\n                    B=" << b << "\n";
            } else if (detallado && t < 5) {
                cout << "  OK  aleatorio #" << t << ":  " << abrev(a) << " x " << abrev(b) << " = " << abrev(k) << "\n";
            }
        }
        ++total; if (malas) ++fallos;
        if (malas) cout << "  " << malas << "/" << N_PEQ << " pares fallaron\n";
        else cout << "  " << N_PEQ << " pares aleatorios (1..200 digitos, con ceros y bloques): Karatsuba == clasica\n";
 
        int malas2 = 0;
        for (int t = 0; t < N_GRA; ++t) {
            string a = aleatorio(g, 300 + g() % 1500), b = aleatorio(g, 300 + g() % 1500);
            if (multiplicacionClasica(a, b) != karatsuba(a, b)) ++malas2;
        }
        ++total; if (malas2) ++fallos;
        if (malas2) cout << "  " << malas2 << "/" << N_GRA << " pares grandes fallaron\n";
        else cout << "  " << N_GRA << " pares grandes (300..1800 digitos): OK\n";
    }
 
    cout << "\nResumen: " << (total - fallos) << "/" << total << " comprobaciones OK"
              << (fallos ? "  -> HAY FALLOS\n" : "  -> TODO CORRECTO\n");
    return fallos ? 1 : 0;
}

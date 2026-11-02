#include "karatsuba.h"
#include "bigint_ops.h"
#include "classic.h"

string karatsuba(const string& a, const string& b) {

    size_t n = max(a.size(), b.size());

    // Caso base
    if (n == 1) {
        return multiplicacionClasica(a, b);
    }

    // Ahora sí vamos contruyendo karatsuba progresivamente

    size_t m = n / 2;
    auto [x1, x0] = partir(a, m);
    auto [y1, y0] = partir(b, m);

    string p1 = karatsuba(x1, y1);
    string p2 = karatsuba(x0, y0);
    string p3 = karatsuba(sumar(x1, x0), sumar(y1, y0));
    string middle = restar(restar(p3, p1), p2);

    return sumar(sumar(desplazar(p1, 2 * m), desplazar(middle, m)), p2);
}


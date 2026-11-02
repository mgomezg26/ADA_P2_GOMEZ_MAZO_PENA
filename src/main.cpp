#include <iostream>
#include "karatsuba.h"

using namespace std;

int main() {
    string a = "21312431";
    string b = "12";

    // Mientras karatsuba() no rellene por dentro, se hace aquí:
    size_t l = max(a.size(), b.size());
    size_t n = 1;
    while (n < l) n *= 2;
    string a_padded = string(n - a.size(), '0') + a;
    string b_padded = string(n - b.size(), '0') + b;

    cout << "Resultado: " << karatsuba(a_padded, b_padded) << endl;
    return 0;
}
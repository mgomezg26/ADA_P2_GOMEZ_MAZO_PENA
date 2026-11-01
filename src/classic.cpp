// IMplementación de la multiplicación clásica 

#include "classic.h"
#include <vector>

using namespace std;
 
string multiplicacionClasica(const string& a, const string& b) {
    const size_t n = a.size(), m = b.size();
    if (n == 0 || m == 0) return "0";
    vector<int> acc(n + m, 0);   // acc[k] = coeficiente de 10^k (k=0 es la unidad)
    for (size_t i = 0; i < n; ++i) {
        int da = a[n - 1 - i] - '0';
        if (da == 0) continue;
        for (size_t j = 0; j < m; ++j)
            acc[i + j] += da * (b[m - 1 - j] - '0');
    }
    int carry = 0;
    for (size_t k = 0; k < n + m; ++k) {
        int v = acc[k] + carry;
        acc[k] = v % 10;
        carry = v / 10;
    }

    // aqui se construye la cadena (más significativo primero) saltando ceros a la izquierda
    size_t top = n + m;
    while (top > 1 && acc[top - 1] == 0) --top;
    string r;
    r.reserve(top);
    for (size_t k = top; k-- > 0;) r.push_back('0' + acc[k]);
    return r;
}

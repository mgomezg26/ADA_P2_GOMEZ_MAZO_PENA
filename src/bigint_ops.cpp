#include "bigint_ops.h"

string sumar(string a, string b) {

    // La suma se hace de derecha a izquierda por lo que empezamos con el tamaño de los strings
    int i = a.size() - 1;
    int j = b.size() - 1;

    // Inicializamos la variable carry en 0 y el string resultado vacío
    int carry = 0;
    string resultado;
    
    // Repetimos el ciclo mientras haya dígitos en a o b o haya un carry pendiente
    while (i >= 0 || j >= 0 || carry > 0) {
        int digitoA = i >= 0 ? a[i--] - '0' : 0;
        int digitoB = j >= 0 ? b[j--] - '0' : 0;

        int suma = digitoA + digitoB + carry;
        resultado.push_back(char('0' + suma % 10));
        carry = suma / 10;
    }

    // Al final del ciclo, el string resultado contiene la suma en orden inverso
    reverse(resultado.begin(), resultado.end());
    return resultado;
}

int main() {

    string a = "0";
    string b = "20";
 
    cout << sumar(a, b) << endl;
}


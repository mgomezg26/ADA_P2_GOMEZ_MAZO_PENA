#include "bigint_ops.h"
using namespace std;

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

    // Eliminamos los 0's a la izquierda del resultado
    size_t pos = resultado.find_first_not_of('0');
    if (pos != string::npos) {
        resultado = resultado.substr(pos);
    } else {
        resultado = "0";
    }

    return resultado;
}

// Asumimos que a >= b y que ambos son números no negativos
// La resta se hace de derecha a izquierda, similar a la suma, pero considerando el préstamo cuando sea necesario
// Los 0's a la izquierda del resultado se eliminan al final
string restar(string a, string b) {

    // Cubrimos casos base ahorrando cálculos innecesarios
    if (a == b) {
        return "0";
    }

    // Para ir de derecha a izquierda
    int i = a.size() - 1;
    int j = b.size() - 1;

    int borrow = 0;
    string resultado;

    // Repetimos el ciclo mientras haya dígitos en a o b
    while (i >= 0 || j >= 0) {
        int digitoA = i >= 0 ? a[i--] - '0' : 0;
        int digitoB = j >= 0 ? b[j--] - '0' : 0;

        int resta = digitoA - digitoB - borrow;
        // Si la resta es negativa, necesitamos pedir prestado del siguiente dígito
        if (resta < 0) {
            resta += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        resultado.push_back(char('0' + resta));
    }

    reverse(resultado.begin(), resultado.end());

    // Eliminamos los 0's a la izquierda del resultado
    size_t pos = resultado.find_first_not_of('0');

    // Si no encontramos ningún dígito distinto de 0, el resultado es 0 (se manejan los casos donde a=b)
    if (pos != string::npos) {
        resultado = resultado.substr(pos);
    } else {
        resultado = "0";
    }

    return resultado;
}

// Desplaza el número a la izquierda agregando n ceros al final
string desplazar(string a, int n) {
    if (n <= 0) return a;
    if (a == "0") return "0";
    a.append(n, '0');
    return a;
}

// Separa a en mitad alta y mitad baja, devuelve dos mitades
pair<string, string> partir(string a, int n) {

    if (a.size() <= static_cast<size_t>(n)) {
        return make_pair("0", a);
    }

    string alta =  a.substr(0, a.size() - n);
    string baja = a.substr(a.size() - n);
    return make_pair(alta, baja);
}


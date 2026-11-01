#include <string>
#include <iostream>
#include <algorithm>
#include <utility>
#pragma once

using namespace std;

// Funcion para sumar utilizando strings
string sumar(string a, string b);

// Funcion para restar utilizando strings
string restar(string a, string b);

// Función para agregar ceros a la izquierda (Para multiplicar B*10^k)
string desplazar(string a, int k);

// Función para partir un número en dos partes (divide y vencerás)
pair<string, string> partir(string a, int k);
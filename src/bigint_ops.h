#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <utility>

// Funcion para sumar utilizando strings
std::string sumar(std::string a, std::string b);

// Funcion para restar utilizando strings
std::string restar(std::string a, std::string b);

// Función para agregar ceros a la izquierda (Para multiplicar B*10^k)
std::string desplazar(std::string a, int k);

// Función para partir un número en dos partes (divide y vencerás)
std::pair<std::string, std::string> partir(std::string a, int k);
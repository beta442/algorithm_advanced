#pragma once

#include "pch.h"
#include "../../../commonLibs/GCD/gcd.h"

void PrintCoprimeNumbersNormalizedSequenceOfN(const int n, const short amountOfNumbers, std::ofstream& toFile);
std::vector<int> GetNumberDivisors(const int number);
std::vector<std::vector<bool>> GetCoprimeMatrixOfIntVector(const std::vector<int>& vec);
std::vector<std::vector<double>> GetMinMulProductOfIntVector(const std::vector<int>& vec, const int amountOfNumbers, const int upperNumber);

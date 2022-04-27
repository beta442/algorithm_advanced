/* Газизьянов Р.Р. ПС-21, 2022 */

/* Visual studio 2022
 * Platform toolset - v141
 * Windows SDK - 10.0.19041.0 - если компилить проект, то попробуйте изменить на имеющийся Windows SDK
 */

/* 5.4. Число делителей (8)
 * Будем называть нормальным набор из k чисел (a1, a2, …, ak), если выполнены следующие условия:
 * 1)	каждое из чисел ai является делителем числа N;
 * 2)	выполняется неравенство a1 < a2 < … < ak;
 * 3)	числа ai и ai+1 для всех i от 1 до k – 1 являются взаимно простыми;
 * 4)	произведение a1a2…ak не превышает N.
 * Например, набор (2, 9, 10) является нормальным набором из 3 делителей числа 360.
 * Требуется написать программу, которая по заданным значениям N и k определяет количество нормальных наборов из k делителей числа N.
 * Ввод из файла INPUT.TXT. Первая строка входного файла содержит два целых числа: N и k (2 ≤ N ≤ 108, 2 ≤ k ≤ 10).
 * Вывод в файл OUTPUT.TXT. В выходном файле должно содержаться одно число — количество нормальных наборов из k делителей числа N.
 */

#include "headers/pch.h"

struct Args
{
	int n;
	short divisorsAmount;
};

struct ParsedArgs
{
	std::optional<Args> args;
	std::string message;
};

ParsedArgs ParseArgs(std::ifstream& inputFile, std::ofstream& outputFile)
{
	ParsedArgs parsedArgs{ std::nullopt, "" };

	if (inputFile.bad())
	{
		std::cout << "Something bad with input file" << std::endl;
		return parsedArgs;
	}
	if (outputFile.bad())
	{
		std::cout << "Something bad with output fail" << std::endl;
		return parsedArgs;
	}
	if (inputFile.eof())
	{
		parsedArgs.message = "Wrong arguments count";
	}
	int n;
	inputFile >> n;
	if (inputFile.fail() || (n < 2 || n > 10e8))
	{
		parsedArgs.message = "Wrong input N parameter";
		return parsedArgs;
	}

	if (inputFile.eof())
	{
		parsedArgs.message = "Wrong arguments count";
	}
	short divisorsAmount;
	inputFile >> divisorsAmount;
	if (inputFile.fail() || (divisorsAmount < 2 || divisorsAmount > 10))
	{
		parsedArgs.message = "Wrong input k parameter";
		return parsedArgs;
	}

	Args args{ n, divisorsAmount };
	parsedArgs.args = args;

	return parsedArgs;
}

int GCD(int x, int y)
{
	if (x == INT_MIN || y == INT_MIN)
	{
		throw new std::out_of_range("One or two arguments are out of range");
	}
	x = std::abs(x);
	y = std::abs(y);
	while (x != 0 && y != 0)
	{
		y %= x;
		std::swap(x, y);
	}

	return (y != 0) ? y : 1;
}

void PrintCoprimeNumbersNormalizedSequenceOfN(const int n, const short amountOfNumbers, std::ofstream& toFile)
{
	if (n <= 0 || amountOfNumbers <= 0)
	{
		toFile << "N and k should be positive natural numbers" << std::endl;
		return;
	}
	std::vector<int> divisors;

	int sqrtN = static_cast<int>(std::floor(std::sqrt(n)));
	for (int d = 1; d <= sqrtN; ++d)
	// получим список делителей до корня N
	{
		if (n % d == 0)
		{
			divisors.push_back(d);
		}
	}
	for (int i = static_cast<int>(std::size(divisors) - 1); i >= 0; --i)
	// получим список делителей (от корня N) до (N)
	{
		if (divisors[i] * divisors[i] != n)
		{
			divisors.push_back(n / divisors[i]);
		}
	}

	int divisorsAmount = static_cast<int>(std::size(divisors));
	std::vector<std::vector<bool>> coprime(divisorsAmount, std::vector<bool>(divisorsAmount));
	for (int i = 0; i < divisorsAmount; ++i)
	// создадим матрицу, по которой можно будет проверить - являются ли числа взаимнопростыми
	{
		for (int j = i; j < divisorsAmount; ++j)
		{
			coprime[i][j] = GCD(divisors[i], divisors[j]) == 1;
			coprime[j][i] = coprime[i][j];
		}
	}

	std::vector<std::vector<double>> minProduct(amountOfNumbers + 1, std::vector<double>(divisorsAmount + 1));
	for (int i = 1; i <= amountOfNumbers; ++i)
	{
		for (int j = 0; j <= divisorsAmount; ++j)
		{
			if (j == divisorsAmount)
			{
				minProduct[i][j] = n + 1;
			}
			else if (i == 1)
			{
				minProduct[i][j] = divisors[j];
			}
			else
			{
				minProduct[i][j] = minProduct[i - 1][j + 1] * divisors[j];
			}
		}
	}

	int total = 0;
	std::function<void(int, int, int)> count = [&](int lastDivider, int p, int rem) {
		if (rem == 0)
		{
			total++;
			return;
		}
		for (int currDivider = lastDivider + 1; currDivider < divisorsAmount && p * minProduct[rem][currDivider] <= n; ++currDivider)
		{
			if (coprime[lastDivider][currDivider])
			{
				count(currDivider, p * divisors[currDivider], rem - 1);
			}
		}
	};

	for (int i = 0; i < divisorsAmount; ++i)
	{
		count(i, divisors[i], amountOfNumbers - 1);
	}

	toFile << total << std::endl;
}

int main()
{
	std::ifstream inputFile{ "input.txt" };
	std::ofstream outputFile{ "output.txt" };

	ParsedArgs parsedArgs = ParseArgs(inputFile, outputFile);
	if (!parsedArgs.args.has_value())
	{
		std::cout << parsedArgs.message << std::endl;
		return 1;
	}

	int n = parsedArgs.args.value().n;
	short divisorsAmount = parsedArgs.args.value().divisorsAmount;

	PrintCoprimeNumbersNormalizedSequenceOfN(n, divisorsAmount, outputFile);

	return 0;
}

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
#include "headers/taskLib.h"

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

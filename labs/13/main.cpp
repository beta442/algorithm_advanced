/* Газизьянов Р.Р. ПС-21, 2022 */

/* Visual studio 2022
 * Platform toolset - v141
 * Windows SDK - 10.0.19041.0 - если компилить проект, то попробуйте изменить на имеющийся Windows SDK
 */

/* 13.2 Алгоритм Кнута – Морриса – Пратта (6)
 * Требуется найти все вхождения образца в текстовом файле методом КМП.
 * В файле нет переноса слов. Образец может включать пробелы и переходить с одной строки файла на другую.
 * Конец строки файла может интерпретироваться как пробел.
 * Результаты поиска не должны зависеть от регистра букв,
 * то есть каждая буква в образце и файле может быть как строчной, так и прописной.
 *
 * Ввод из файла INPUT.TXT. Первая строка файла является образцом и имеет длину от 1 до 255.
 * Вторая строка задает имя текстового файла.
 * Вывод в файл OUTPUT.TXT.
 * Вывести в каждой строке через пробел последовательность номеров строк и позиций в строке,
 * начиная с которых образец входит в текст. Нумерация строк и позиций в строке начинается с 1.
 * Если вхождений нет, вывести No.
 */

#include "headers/pch.h"

struct Args
{
	std::string needle;
	std::string toSearchFileName;
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
	std::string needle;
	inputFile >> needle;
	if (inputFile.fail() || (std::size(needle) < 1 || std::size(needle) > 255))
	{
		parsedArgs.message = "Wrong input needle parameter. Length should be between 1 and 255";
		return parsedArgs;
	}

	if (inputFile.eof())
	{
		parsedArgs.message = "Wrong arguments count";
	}
	std::string fileName;
	fileName.clear();
	inputFile >> fileName;
	if (inputFile.fail() || std::size(fileName) == 0)
	{
		parsedArgs.message = "Wrong input fileName parameter";
		return parsedArgs;
	}

	Args args{ needle, fileName };
	parsedArgs.args = args;

	return parsedArgs;
}

void PrintLineAndPosAtLineByKMP(std::ifstream& searchFile, const std::string needle, std::ostream& outputFile)
{

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

	std::string needle = parsedArgs.args.value().needle,
				searchFileName = parsedArgs.args.value().toSearchFileName;
	std::ifstream searchFile{ searchFileName };
	if (!searchFile.is_open() || searchFile.bad())
	{
		std::cout << "Something bad with search file" << std::endl;
		return 1;
	}

	PrintLineAndPosAtLineByKMP(searchFile, needle, outputFile);

	return 0;
}

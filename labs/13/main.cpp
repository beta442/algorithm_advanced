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

constexpr auto MAX_NEEDLE_SIZE = 255;

ParsedArgs ParseArgs(std::ifstream& inputFile, std::ofstream& outputFile)
{
	ParsedArgs parsedArgs{ std::nullopt, "" };

	if (!inputFile.is_open() || inputFile.bad())
	{
		std::cout << "Something bad with input file" << std::endl;
		return parsedArgs;
	}
	if (!outputFile.is_open() || outputFile.bad())
	{
		std::cout << "Something bad with output fail" << std::endl;
		return parsedArgs;
	}
	std::string needle;
	if (inputFile.eof() || !std::getline(inputFile, needle))
	{
		parsedArgs.message = "Wrong arguments count";
	}
	if (inputFile.fail() || (std::size(needle) < 1 || std::size(needle) > MAX_NEEDLE_SIZE))
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

// реализация префикс функции для заданной строки
std::vector<int> PrefixFunction(const std::string& needle)
{
	int prevLongestPrefixSuffix = 0;

	size_t i = 1, patternSize = std::size(needle);
	std::vector<int> prefix(patternSize);
	prefix[0] = 0;
	while (i < patternSize)
	{
		if (needle[i] == needle[prevLongestPrefixSuffix])
		{
			prevLongestPrefixSuffix++;
			prefix[i] = prevLongestPrefixSuffix;
			i++;
		}
		else
		{
			if (prevLongestPrefixSuffix != 0)
			{
				prevLongestPrefixSuffix = prefix[prevLongestPrefixSuffix - 1];
			}
			else
			{
				prefix[i] = 0;
				i++;
			}
		}
	}
	return prefix;
}

std::string StringToLower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return s;
}

struct FilePos
{
	size_t lineIndex, pos;
};

std::vector<FilePos> result;

// 0 - поиск окночился ошибкой, else - требуется добавить к string продолжение, чтобы посмотреть, можно ли найти вхождение дальше
size_t KMPSearch(const std::string& string, // строка для анализа
	const std::vector<int>& prefix, // массив переходов
	const std::string& needle, // строка поиска
	const size_t lineIndex, // индекс строки для анализа в файле, нумерация от 1
	const size_t offset) // передать смещение для печати позиции в строке, используется в случае, когда поиск мог бы быть продолжен
{
	std::string lowerString = StringToLower(string);
	std::string lowerNeedle = StringToLower(needle);

	size_t searchStringLength = std::size(lowerString), i = 0, j = 0, needleSize = std::size(lowerNeedle);
	while (i < searchStringLength)
	{
		if (lowerNeedle[j] == lowerString[i])
		{
			j++;
			i++;
		}

		if (j == needleSize)
		{
			result.push_back({ lineIndex, i + 1 - j + (offset ? offset : 0) });
			j = prefix[j - 1];
		}
		else if (i < searchStringLength && lowerNeedle[j] != lowerString[i])
		{
			if (j != 0)
			{
				j = prefix[j - 1];
			}
			else
			{
				++i;
			}
		}
	}
	return j;
}

void PrintLineAndPosAtLineByKMP(std::ifstream& searchFile, const std::string needle, std::ostream& outputFile)
{
	std::vector<int> prefix = PrefixFunction(needle);

	std::string searchString, buffer;
	size_t lineIndex = 1, collectedNeedleIndex, offset = 0;
	const size_t needleSize = std::size(needle);
	bool needToRead = true;
	while (!searchFile.eof())
	{
		if (needToRead)
		{
			std::getline(searchFile, searchString);
		}

		collectedNeedleIndex = KMPSearch(searchString, prefix, needle, needToRead ? lineIndex : lineIndex - 1, offset);

		if (collectedNeedleIndex != 0 && !searchFile.eof() && std::getline(searchFile, buffer))
		{
			offset = std::size(searchString) - collectedNeedleIndex;
			searchString = searchString.substr(offset) + " " + buffer;
			needToRead = false;
		}
		else
		{
			offset = 0;
			needToRead = true;
		}
		++lineIndex;
	}

	if (result.size() == 0)
	{
		outputFile << "No" << std::endl;
	}
	for (size_t i = 0; i < result.size(); ++i)
	{
		std::cout << result[i].lineIndex << " " << result[i].pos << std::endl;
		outputFile << result[i].lineIndex << " " << result[i].pos << std::endl;
	}
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

	setlocale(0, "");
	PrintLineAndPosAtLineByKMP(searchFile, needle, outputFile);

	inputFile.close();
	outputFile.close();
	searchFile.close();

	return 0;
}

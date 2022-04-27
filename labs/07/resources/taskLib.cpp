#include "../headers/taskLib.h"

void PrintCoprimeNumbersNormalizedSequenceOfN(const int n, const short amountOfNumbers, std::ofstream& toFile)
{
	if (n <= 0 || amountOfNumbers <= 0)
	{
		toFile << "N and k should be positive natural numbers" << std::endl;
		return;
	}

	// ������� ������ ��������� ����� n
	std::vector<int> divisors = GetNumberDivisors(n);

	// "��������������" ������������ � ���� �������, ��� ������ ������/������� coprime ������������� ������� �������� divisors
	std::vector<std::vector<bool>> coprime = GetCoprimeMatrixOfIntVector(divisors);

	// ������� �������, � ������� ������� ������ �������� �������������� ����� ������ �� N, ���������: � ����� �� �� ��������� ������?
	std::vector<std::vector<double>> minProduct = GetMinMulProductOfIntVector(divisors, amountOfNumbers, n);

	int total = 0; // ������� ��������� �������
	int divisorsAmount = static_cast<int>(std::size(divisors));
	std::function<void(int, int, int)> count = [&](int lastDividerIndex, int currDivider, int numbersLeft) {
		if (numbersLeft == 0)
		{
			total++;
			return;
		}
		for (int currDividerIndex = lastDividerIndex + 1;
			 currDividerIndex < divisorsAmount && // ���������� ��� ��������
			 currDivider * minProduct[numbersLeft][currDividerIndex] <= n; // ����� �� �������� ������?
			 ++currDividerIndex)
		{
			if (coprime[lastDividerIndex][currDividerIndex])
			// ��������� ��������, ������� ������
			{
				count(currDividerIndex, currDivider * divisors[currDividerIndex], numbersLeft - 1);
			}
		}
	};

	for (int i = 0; i < divisorsAmount; ++i)
	{
		count(i, divisors[i], amountOfNumbers - 1);
	}

	toFile << total << std::endl;
}

std::vector<int> GetNumberDivisors(const int number)
{
	std::vector<int> result;

	int sqrtN = static_cast<int>(std::floor(std::sqrt(number)));
	for (int d = 1; d <= sqrtN; ++d)
	// ������� ������ ��������� �� ����� N
	{
		if (number % d == 0)
		{
			result.push_back(d);
		}
	}
	for (int i = static_cast<int>(std::size(result) - 1); i >= 0; --i)
	// ������� ������ ��������� (�� ����� N) �� (N)
	{
		if (result[i] * result[i] != number)
		{
			result.push_back(number / result[i]);
		}
	}

	return result;
}

std::vector<std::vector<bool>> GetCoprimeMatrixOfIntVector(const std::vector<int>& vec)
{
	int divisorsAmount = static_cast<int>(std::size(vec));
	std::vector<std::vector<bool>> coprime(divisorsAmount, std::vector<bool>(divisorsAmount));
	for (int i = 0; i < divisorsAmount; ++i)
	// �������� �������, �� ������� ����� ����� ��������� - �������� �� ����� ���������������
	{
		for (int j = i; j < divisorsAmount; ++j)
		{
			coprime[i][j] = GCD(vec[i], vec[j]) == 1;
			coprime[j][i] = coprime[i][j];
		}
	}

	return coprime;
}

std::vector<std::vector<double>> GetMinMulProductOfIntVector(const std::vector<int>& vec,
	const int amountOfNumbers, const int upperNumber)
{
	int divisorsAmount = static_cast<int>(std::size(vec));
	std::vector<std::vector<double>> minProduct(amountOfNumbers + 1, std::vector<double>(divisorsAmount + 1));
	for (int i = 1; i <= amountOfNumbers; ++i)
	{
		for (int j = 0; j <= divisorsAmount; ++j)
		{
			if (j == divisorsAmount)
			// ������� ������
			{
				minProduct[i][j] = upperNumber + 1;
			}
			else if (i == 1)
			// ������� ������
			{
				minProduct[i][j] = vec[j];
			}
			else
			// ���������
			{
				minProduct[i][j] = minProduct[i - 1][j + 1] * vec[j];
			}
		}
	}

	return minProduct;
}

#include "gcd.h"

#include <iostream>

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

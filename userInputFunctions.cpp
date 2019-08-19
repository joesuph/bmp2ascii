#include <iostream>
#include "userInputFunctions.h"

void setFileName(char filename[])
{
	std::cout << "What bmp file would you like to convert? ";
	std::cin >> filename;
}

void setMaxLengths(int maxLengths[])
{
	std::cout << "Max output width: ";
	std::cin >> maxLengths[0];
	std::cout << "Max output height: ";
	std::cin >> maxLengths[1];
}
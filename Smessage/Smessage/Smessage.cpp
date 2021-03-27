// Smessage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
#include <bitset>

#include "WaveReader.hpp"

#include "Wave.hpp"

#define SECTIONDIVIDER "-------------------------------------------"

#define WAVFILE "D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/de-oude-schicht.wav"
#define AIFFFile "D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/infinitely-many-numbers.aiff"
#define CAFFILE "D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/news.caf"

void read();
void write();

int main()
{
	int option = 0;

	std::cout << "Welcome to Smessage!" << std::endl;
	std::cout << SECTIONDIVIDER << std::endl;

	std::cout << "What do you wanna do?" << std::endl
		<< "1. Read secret message" << std::endl
		<< "2. Write secret message" << std::endl;

	bool tried = false;

	while (option != 1 && option != 2)
	{
		if (tried)
		{
			std::cout << "Invalid option! Please enter \"1\" or \"2\"." << std::endl;
		}

		std::cin >> option;
		std::cin.get();

		tried = true;

		std::cout << SECTIONDIVIDER << std::endl;
	}

	if (option == 1)
	{
		read();
	}
	else if (option == 2)
	{
		write();
	}

	return 0;
}

void read()
{
	std::string filepath;

	std::cout << "Insert path to a wav file: ";
	std::cin >> filepath;
	std::cin.get();

	std::cout << SECTIONDIVIDER << std::endl;

	//Look for message in file
	std::unique_ptr<WaveReader> fileReader = std::make_unique<WaveReader>();
	std::vector<std::bitset<8>> possibleMessage = fileReader.get()->readFile(filepath);

	//Convert bitset<8> to text
	for (int i = 0; i < possibleMessage.size(); i++)
	{
		unsigned long x = possibleMessage[i].to_ulong();
		unsigned char y = static_cast<unsigned char>(x);

		std::cout << y;
	}
}

void write()
{
	std::cout << "This part has not been developed yet!" << std::endl;
}
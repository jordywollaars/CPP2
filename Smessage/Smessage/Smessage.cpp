// Smessage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
#include <bitset>

#include "WaveHandler.hpp"

#define SECTIONDIVIDER "-------------------------------------------"

#define WAVFILEWITHMESSAGE "D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/de-oude-schicht.wav"
#define WAVFILEWITHOUTMESSAGE "D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/sfx_weapon_singleshot2.wav"
#define AIFFFile "D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/infinitely-many-numbers.aiff"
#define CAFFILE "D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/news.caf"

void read();
void write(std::string filepath = "");

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
	std::unique_ptr<WaveHandler> fileReader = std::make_unique<WaveHandler>();
	std::vector<std::bitset<8>> possibleMessage = fileReader.get()->readFile(filepath);

	bool invalidMessage = false;
	std::string message = "";

	//Validate message
	{
		for (int i = 0; i < possibleMessage.size(); i++)
		{
			unsigned long x = possibleMessage[i].to_ulong();
			char c;
			if (x <= CHAR_MAX) {
				c = static_cast<char>(x);
				message += c;
			}
			else {
				invalidMessage = true;
			}
		}
	}

	if (!invalidMessage)
	{
		std::cout << message << std::endl;
	}
	else
	{
		std::cout << "No message was found! Did you mean to hide a message? (y/n)" << std::endl;

		std::string input;

		while (input != "y" && input != "n")
		{
			std::cin >> input;
			std::cin.get();
		}

		if (input == "y")
		{
			write(filepath);
		}
	}
}

void write(std::string filepath)
{
	std::string messageToHide;

	std::cout << "Insert message to hide: ";
	std::getline(std::cin, messageToHide);

	std::unique_ptr<WaveHandler> fileHandler = std::make_unique<WaveHandler>();
	fileHandler.get()->setMessageToHide(messageToHide);

	while (filepath.size() <= 0)
	{
		std::cout << "Insert path to a wav file: ";
		std::cin >> filepath;
		std::cin.get();
	}

	std::cout << SECTIONDIVIDER << std::endl;

	fileHandler.get()->writeMessageInFile(filepath);

}
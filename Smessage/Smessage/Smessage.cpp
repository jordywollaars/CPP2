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

void read(std::unique_ptr<WaveHandler>& fileHandler);
void write(std::unique_ptr<WaveHandler>& fileHandler);

int main()
{
	char option = '0';

	while (option != '3')
	{
		system("cls");
		option = '0';
		std::cout << "Welcome to Smessage!" << std::endl;
		std::cout << SECTIONDIVIDER << std::endl;

		std::cout << "What do you wanna do?" << std::endl
			<< "1. Read secret message" << std::endl
			<< "2. Write secret message" << std::endl
			<< "3. Quit" << std::endl;

		bool tried = false;

		while (option != '1' && option != '2' && option != '3')
		{
			if (tried)
			{
				std::cout << "Invalid option! Please enter \"1\", \"2\" or \"3\"." << std::endl;
				option = 0;
			}

			std::cin >> option;
			std::cin.get();

			tried = true;

			std::cout << SECTIONDIVIDER << std::endl;
		}

		if (option == '1' || option == '2')
		{
			std::unique_ptr<WaveHandler> fileReader = std::make_unique<WaveHandler>();

			if (option == '1')
			{
				try
				{
					read(fileReader);
				}
				catch (const std::exception& e)
				{
					std::cerr << "Oops, it seems like something went wrong: " << e.what() << std::endl;
				}

				system("pause");
			}
			else if (option == '2')
			{
				try
				{
					write(fileReader);
				}
				catch (const std::exception& e)
				{
					std::cerr << "Oops, it seems like something went wrong: " << e.what() << std::endl;
				}

				system("pause");
			}
		}
	}

	return 0;
}

void read(std::unique_ptr<WaveHandler>& fileHandler)
{
	std::string filepath;

	std::cout << "Insert path to a wav file: ";
	std::cin >> filepath;
	std::cin.get();

	fileHandler.get()->setOperatingPath(filepath);

	std::cout << SECTIONDIVIDER << std::endl;

	std::vector<std::bitset<8>> possibleMessage;

	possibleMessage = fileHandler.get()->readFile();

	bool invalidMessage = false;
	std::string message = "";

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
			write(fileHandler);
		}
	}
}

void write(std::unique_ptr<WaveHandler>& fileHandler)
{
	std::string messageToHide;

	std::cout << "Insert message to hide: ";
	std::getline(std::cin, messageToHide);

	fileHandler.get()->setMessageToHide(messageToHide);

	while (fileHandler.get()->getOperatingPath().size() <= 0)
	{
		std::string filepath;

		std::cout << "Insert path to a wav file: ";
		std::cin >> filepath;
		std::cin.get();

		fileHandler.get()->setOperatingPath(filepath);
	}

	std::cout << SECTIONDIVIDER << std::endl;

	try
	{
		fileHandler.get()->writeMessageInFile();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Something went wrong: " << e.what() << std::endl;
		return;
	}

}
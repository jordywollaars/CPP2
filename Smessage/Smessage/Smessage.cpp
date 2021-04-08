// Smessage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <memory>
#include <iostream>

#include <crtdbg.h>

#include "WaveHandler.hpp"
#include "AIFFHandler.hpp"
#include "CAFHandler.hpp"

#define SECTIONDIVIDER "-------------------------------------------"

//D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/de-oude-schicht.wav
//D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/24_bit_fixed.wav
//D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/32_bit_float.wav
//D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/sfx_weapon_singleshot2.wav
//D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/infinitely-many-numbers.aif
//D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/test.aif
//D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/news.caf

std::unique_ptr<FileHandler> getSpecificFileHandler(char extension);
void start();
void read(const std::unique_ptr<FileHandler>& fileHandler);
void write(const std::unique_ptr<FileHandler>& fileHandler);

int main()
{
	try
	{
		start();
	}
	catch (...) //Catch any missed errors
	{
		std::cerr << "unknown error \n";
		return -1;
	}

	//Log memoryleaks if present
	_CrtDumpMemoryLeaks();

	return 0;
}

void start()
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

		//Promt until valid input is received
		while (option != '1' && option != '2' && option != '3')
		{
			if (tried)
			{
				std::cout << "Invalid option! Please enter \"1\", \"2\" or \"3\"." << std::endl;
				option = '0';
			}

			std::cin >> option;

			tried = true;

			std::cout << SECTIONDIVIDER << std::endl;
		}

		if (option == '1' || option == '2')
		{
			tried = false;
			char extension = '0';

			std::cout << "What type of file are you using?" << std::endl
				<< "1. WAV" << std::endl
				<< "2. AIFF" << std::endl
				<< "3. CAF" << std::endl;

			//Promt until valid input is received
			while (extension != '1' && extension != '2' && extension != '3')
			{
				if (tried)
				{
					std::cout << "Invalid option! Please enter \"1\", \"2\" or \"3\"." << std::endl;
					extension = '0';
				}

				std::cin >> extension;

				tried = true;

				std::cout << SECTIONDIVIDER << std::endl;
			}

			//Get required file handler
			const std::unique_ptr<FileHandler> fileReader = getSpecificFileHandler(extension);

			try
			{
				if (option == '1')
				{
					read(fileReader);
				}
				else if (option == '2')
				{
					write(fileReader);
				}
			}
			catch (const std::exception& e)
			{
				std::cout << SECTIONDIVIDER << std::endl;
				std::cerr << "Oops, it seems like something went wrong: " << e.what() << std::endl;
			}
			system("pause");
		}
	}
}

void read(const std::unique_ptr<FileHandler>& fileHandler)
{
	std::string filepath;

	std::cout << "Insert path to a " << fileHandler.get()->getExtensionString() << " file: ";
	std::cin >> filepath;
	std::cin.get();

	fileHandler.get()->setOperatingPath(filepath);

	std::cout << SECTIONDIVIDER << std::endl;

	const std::vector<std::bitset<8>> possibleMessage = fileHandler.get()->readFile();

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

void write(const std::unique_ptr<FileHandler>& fileHandler)
{
	std::string messageToHide;

	std::cout << "Insert message to hide: ";
	std::getline(std::cin, messageToHide);

	fileHandler.get()->setMessageToHide(messageToHide);

	while (fileHandler.get()->getOperatingPath().size() <= 0)
	{
		std::string filepath;

		std::cout << "Insert path to a " << fileHandler.get()->getExtensionString() << " file: ";
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

std::unique_ptr<FileHandler> getSpecificFileHandler(char extension)
{
	switch (extension)
	{
	case '1':
		return std::make_unique<WaveHandler>();
	case '2':
		return std::make_unique<AIFFHandler>();
	case '3':
		return std::make_unique<CAFHandler>();
	}

	return NULL;
}
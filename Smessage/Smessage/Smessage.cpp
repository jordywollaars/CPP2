// Smessage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <ios>
#include <vector>

#define WAVFILE "D:/Users/Jordy/Desktop/CPP2/Smessage/de-oude-schicht.wav"
#define AIFFFile "D:/Users/Jordy/Desktop/CPP2/Smessage/infinitely-many-numbers.aiff"
#define CAFFILE "D:/Users/Jordy/Desktop/CPP2/Smessage/news.caf"

int main()
{
	std::ifstream input(WAVFILE, std::ios::binary);

	// copies all data into buffer
	std::vector<uint16_t> buffer(std::istreambuf_iterator<char>(input), {});

	int i = 0;
	while (i < buffer.size())
	{
		std::cout << std::hex << buffer[i] << " ";

		i++;
	}

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

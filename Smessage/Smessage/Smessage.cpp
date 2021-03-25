// Smessage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
#include <bitset>

#define WAVFILE "D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/de-oude-schicht.wav"
#define AIFFFile "D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/infinitely-many-numbers.aiff"
#define CAFFILE "D:/Users/Jordy/Desktop/CPP2/CPP2/Smessage/news.caf"

int main()
{
	std::ifstream input(WAVFILE, std::ios::binary);

	// copies all data into buffer
	std::vector<char> buffer(std::istreambuf_iterator<char>(input), {});

	std::vector<std::bitset<8>> possibleMessage;

	std::vector<std::bitset<8>> bits;

	for (int i = 44; i < buffer.size(); i+=2)
	{
		char shifted = buffer[i] << 7;
		std::bitset<8> x(shifted);
		//std::cout << (x >> 7) << std::endl;

		/*bool bit = shifted >> 7;

		std::cout << bit << std::endl;*/


		bits.push_back(x);

		if (bits.size() == 8)
		{
			for (int i = 0; i < 8; i++)
			{
				std::bitset<8> x(bits[i]);
				std::bitset<8> test = x >> i;

				//std::cout << test << std::endl;
				bits[i] = test;
			}

			possibleMessage.push_back(
				bits[0] | bits[1] | bits[2] | bits[3] | bits[4] | bits[5] | bits[6] | bits[7]
			);

			std::bitset<8> x(possibleMessage[possibleMessage.size()-1]);

			std::cout << x << std::endl;

			if (x == 0)
			{
				break;
			}

			bits.clear();
		}
	}
	
	return 0;
}
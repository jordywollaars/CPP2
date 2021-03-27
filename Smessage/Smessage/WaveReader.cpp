#include "WaveReader.hpp"

#include <iostream>
#include <fstream>
#include <ios>

#include "Wave.hpp"

std::vector<std::bitset<8>> WaveReader::readFile(const std::string filepath)
{
	int startPosition = determineHeaderSize(filepath);

	std::ifstream input(filepath, std::ios::binary);
	// copies all data into buffer
	std::vector<char> buffer(std::istreambuf_iterator<char>(input), {});

	std::vector<std::bitset<8>> possibleMessage;

	std::vector<std::bitset<8>> bits;

	for (int i = startPosition; i < buffer.size(); i += 2)
	{
		char shifted = buffer[i] << 7;
		std::bitset<8> x(shifted);

		bits.push_back(x);

		if (bits.size() == 8)
		{
			for (int i = 0; i < 8; i++)
			{
				std::bitset<8> x(bits[i]);
				std::bitset<8> test = x >> i;

				bits[i] = test;
			}

			possibleMessage.push_back(
				bits[0] | bits[1] | bits[2] | bits[3] | bits[4] | bits[5] | bits[6] | bits[7]
			);

			std::bitset<8> x(possibleMessage[possibleMessage.size() - 1]);

			if (x == 0)
			{
				break;
			}

			bits.clear();
		}
	}

	return possibleMessage;
}

int WaveReader::determineHeaderSize(const std::string filepath)
{
	Wave waveHeader;
	//TODO: get rid of raw pointer
	FILE* waveFile;
	int headerSize = sizeof(waveHeader);

	fopen_s(&waveFile, filepath.c_str(), "r");

	if (waveFile == NULL)
	{
		std::cout << "Invalid!" << std::endl;
		return -1;
	}

	fread(&waveHeader, headerSize, 1, waveFile);

	fclose(waveFile);

	return headerSize;
}

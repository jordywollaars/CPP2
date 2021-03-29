#include "WaveHandler.hpp"

#include <iostream>
#include <fstream>
#include <ios>

#include "Wave.hpp"

const std::vector<std::bitset<8>> WaveHandler::readFile(const std::string filepath)
{
	std::ifstream input(filepath, std::ios::binary);

	// copies all data into buffer
	std::vector<char> buffer(std::istreambuf_iterator<char>(input), {});

	Wave waveHeader;

	input.read((char*)&waveHeader, sizeof(waveHeader));
	int headerSize = sizeof(waveHeader);

	int startPosition = headerSize;

	input.close();

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

			//std::cout << x << std::endl;

			if (x == 0)
			{
				break;
			}

			bits.clear();
		}
	}

	return possibleMessage;
}

const void WaveHandler::setMessageToHide(std::string message)
{
	this->messageToHide = message;

	for (int i = 0; i < this->messageToHide.size(); i++)
	{
		std::bitset<8> byte(messageToHide[i]);

		this->messageBits.push_back(byte[7]);
		this->messageBits.push_back(byte[6]);
		this->messageBits.push_back(byte[5]);
		this->messageBits.push_back(byte[4]);
		this->messageBits.push_back(byte[3]);
		this->messageBits.push_back(byte[2]);
		this->messageBits.push_back(byte[1]);
		this->messageBits.push_back(byte[0]);
	}
}

void WaveHandler::writeMessageInFile(const std::string filepath)
{
	std::ifstream input(filepath, std::ios::binary);
	// copies all data into buffer
	std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});

	Wave waveHeader;

	input.read((char*)&waveHeader, sizeof(waveHeader));
	int headerSize = sizeof(waveHeader);

	int startPosition = headerSize;

	input.close();

	uint8_t mask1{ 1 };
	uint8_t mask2{ 0xFE };

	int counter = 0;
	for (int i = startPosition; i < buffer.size(); i += 2)
	{
		if (counter < this->messageBits.size())
		{
			if (buffer[i] & mask1)
			{
				if (messageBits[counter] != 1)
				{
					buffer[i] = buffer[i] & mask2;
				}
			}
			else
			{
				if (messageBits[counter] != 0)
				{
					buffer[i] = buffer[i] | mask1;
				}
			}

			std::bitset<8> x(buffer[i]);
			//std::cout << x << " - " << messageBits[counter] << std::endl;

			counter++;
		}
		else
		{
			for (int j = 0; j < 8; j++)
			{
				int bufferPos = j * 2;

				if (buffer[i + bufferPos] & mask1)
				{
					buffer[i + bufferPos] = buffer[i + bufferPos] & mask2;
				}

				std::bitset<8> x(buffer[i+bufferPos]);
				//std::cout << x << " - 0" << std::endl;
			}

			break;
		}
	}

	if (counter < this->messageBits.size())
	{
		std::cout << "Couldn't fit entire message inside the file.";
	}

	std::ofstream ofs;
	ofs.open(filepath, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);

	for (uint8_t byte : buffer)
	{
		char c = byte;
		ofs << c;
	}

	ofs.close();
}

#include "AIFFHandler.hpp"

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>

std::vector<std::bitset<8>> AIFFHandler::readFile() const
{
	const std::vector<char> buffer = this->setupOperatingInfo();

	std::vector<std::bitset<8>> possibleMessage;

	std::vector<std::bitset<8>> bits;

	const std::vector<int> pattern{ 0x53, 0x53, 0x4e, 0x44 };
	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(pattern), std::end(pattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("Data could not be found in the file");
	}

	std::advance(it, 17);

	for (it; it != buffer.end(); std::advance(it, 2))
	{
		//std::cout << "O: " << std::bitset<8>(*it) << std::endl;
		//char shifted = *it >> 7;
		std::bitset<8> x(*it);
		x = x << 7;
		//std::cout << "S: " << x << std::endl;

		bits.push_back(x);

		if (bits.size() == 8)
		{
			for (int i = 0; i < 8; i++)
			{
				std::bitset<8> x(bits[i]);
				std::bitset<8> test = x >> (i);

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

void AIFFHandler::writeMessageInFile() const
{
	std::vector<char> buffer = this->setupOperatingInfo();

	uint8_t mask1{ 1 };
	uint8_t mask2{ 0xFE };

	int counter = 0;
	int nullByteCounter = 8;

	std::vector<int> pattern{ 0x53, 0x53, 0x4e, 0x44 };
	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(pattern), std::end(pattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("Data could not be found in the file");
	}

	std::advance(it, 17);

	//int steps = sampleSize / 8;
	for (it; it != buffer.end(); std::advance(it, 2))
	{
		if (counter < this->messageBits.size())
		{
			if (*it & mask1)
			{
				if (messageBits[counter] != 1)
				{
					*it = *it & mask2;
				}
			}
			else
			{
				if (messageBits[counter] != 0)
				{
					*it = *it | mask1;
				}
			}

			std::bitset<8> x(*it);
			//std::cout << x << " - " << messageBits[counter] << std::endl;

			counter++;
		}
		else if (nullByteCounter >= 0)
		{
			if (*it & mask1)
			{
				*it = *it & mask2;
			}

			std::bitset<8> x(*it);
			//std::cout << x << " - 0" << std::endl;

			nullByteCounter--;
		}
		else
		{
			break;
		}
	}

	if (counter < this->messageBits.size())
	{
		std::cout << "Couldn't fit entire message inside the file." << std::endl;
		return;
	}

	std::ofstream ofs;
	ofs.open(this->operatingPath, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);

	for (uint8_t byte : buffer)
	{
		char c = byte;
		ofs << c;
	}

	ofs.close();
}

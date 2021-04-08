#include "CAFHandler.hpp"

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>

std::vector<std::bitset<8>> CAFHandler::readFile() const
{
	const std::vector<char> buffer = this->setupOperatingInfo();

	std::vector<std::bitset<8>> possibleMessage;

	std::vector<std::bitset<8>> bits;

	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(this->dataPattern), std::end(this->dataPattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("\"data\" could not be found in buffer");
	}

	CAFHandler::AdditionalInfo additionalInfo = this->getAdditionalInfo(buffer);

	if (additionalInfo.flag == 0)
	{
		std::advance(it, 17);
	}
	else
	{
		std::advance(it, 16);
	}

	int steps = additionalInfo.sampleRate / 8;
	for (it; it != buffer.end(); std::advance(it, steps))
	{
		char shifted = *it << 7;
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

void CAFHandler::writeMessageInFile() const
{
	std::vector<char> buffer = this->setupOperatingInfo();

	uint8_t mask1{ 1 };
	uint8_t mask2{ 0xFE };

	int counter = 0;
	int nullByteCounter = 8;

	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(this->dataPattern), std::end(this->dataPattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("\"data\" could not be found in buffer");
	}

	const CAFHandler::AdditionalInfo additionalInfo = this->getAdditionalInfo(buffer);

	if (additionalInfo.flag == 0)
	{
		std::advance(it, 17);
	}
	else
	{
		std::advance(it, 16);
	}

	int steps = additionalInfo.sampleRate / 8;
	for (it; it != buffer.end(); std::advance(it, steps))
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
		throw std::runtime_error("Couldn't fit entire message inside the file.");
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

int CAFHandler::getSampleSizeFromBuffer(const std::vector<char>& buffer) const
{
	auto temp = std::search(std::begin(buffer), std::end(buffer), std::begin(this->descPattern), std::end(this->descPattern));
	if (temp == std::end(buffer))
	{
		throw std::runtime_error("\"desc\" could not be found in the file");
	}

	std::advance(temp, 40);
	unsigned char byte1 = *temp;
	++temp;
	unsigned char byte2 = *temp;
	++temp;
	unsigned char byte3 = *temp;
	++temp;
	unsigned char byte4 = *temp;
	int sampleSize = (byte1 << 24) + (byte2 << 16) + (byte3 << 8) + byte4;

	return sampleSize;
}

const CAFHandler::AdditionalInfo CAFHandler::getAdditionalInfo(const std::vector<char>& buffer) const
{
	auto temp = std::search(std::begin(buffer), std::end(buffer), std::begin(this->descPattern), std::end(this->descPattern));
	if (temp == std::end(buffer))
	{
		throw std::runtime_error("\"desc\" could not be found in the file");
	}

	std::advance(temp, 24);
	unsigned char byte1 = *temp;
	++temp;
	unsigned char byte2 = *temp;
	++temp;
	unsigned char byte3 = *temp;
	++temp;
	unsigned char byte4 = *temp;
	int flag = (byte1 << 24) + (byte2 << 16) + (byte3 << 8) + byte4;

	AdditionalInfo ai{ flag, getSampleSizeFromBuffer(buffer) };
	return ai;
}

const std::string CAFHandler::getExpressionString() const
{
	return "^.*\.(caf|CAF)$";
}

const std::string CAFHandler::getExtensionString() const
{
	return "caf";
}

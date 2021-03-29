#include "WaveHandler.hpp"

#include <iostream>
#include <fstream>
#include <ios>
#include <regex>


#include "Wave.hpp"

std::vector<std::bitset<8>> WaveHandler::readFile() const
{
	OperatingInfo operatingInfo = this->setupOperatingInfo();

	std::vector<std::bitset<8>> possibleMessage;

	std::vector<std::bitset<8>> bits;

	for (int i = operatingInfo.dataStartPosition; i < operatingInfo.buffer.size(); i += 2)
	{
		char shifted = operatingInfo.buffer[i] << 7;
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

void WaveHandler::setMessageToHide(const std::string& message)
{
	this->messageBits.clear();

	for (int i = 0; i < message.size(); i++)
	{
		std::bitset<8> byte(message[i]);

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

void WaveHandler::setOperatingPath(const std::string& filepath)
{
	std::regex expression("^.*\.(wav|WAV)$"); //|caf|CAF|aiff|AIFF

	if (!std::regex_match(filepath, expression))
	{
		throw std::invalid_argument("Invalid file extension.");
	}

	operatingPath = filepath;
}

std::string WaveHandler::getOperatingPath() const
{
	return this->operatingPath;
}

void WaveHandler::writeMessageInFile() const
{
	OperatingInfo operatingInfo = this->setupOperatingInfo();

	uint8_t mask1{ 1 };
	uint8_t mask2{ 0xFE };

	int counter = 0;
	for (int i = operatingInfo.dataStartPosition; i < operatingInfo.buffer.size(); i += 2)
	{
		if (counter < this->messageBits.size())
		{
			if (operatingInfo.buffer[i] & mask1)
			{
				if (messageBits[counter] != 1)
				{
					operatingInfo.buffer[i] = operatingInfo.buffer[i] & mask2;
				}
			}
			else
			{
				if (messageBits[counter] != 0)
				{
					operatingInfo.buffer[i] = operatingInfo.buffer[i] | mask1;
				}
			}

			std::bitset<8> x(operatingInfo.buffer[i]);
			//std::cout << x << " - " << messageBits[counter] << std::endl;

			counter++;
		}
		else
		{
			for (int j = 0; j < 8; j++)
			{
				int bufferPos = j * 2;

				if (operatingInfo.buffer[i + bufferPos] & mask1)
				{
					operatingInfo.buffer[i + bufferPos] = operatingInfo.buffer[i + bufferPos] & mask2;
				}

				std::bitset<8> x(operatingInfo.buffer[i+bufferPos]);
				//std::cout << x << " - 0" << std::endl;
			}

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

	for (uint8_t byte : operatingInfo.buffer)
	{
		char c = byte;
		ofs << c;
	}

	ofs.close();
}

OperatingInfo WaveHandler::setupOperatingInfo() const
{
	std::ifstream input(this->operatingPath, std::ios::binary);

	if (!input)
	{
		throw std::runtime_error("Could not open file");
	}

	// copies all data into buffer
	std::vector<char> buffer(std::istreambuf_iterator<char>(input), {});

	Wave waveHeader;

	input.read((char*)&waveHeader, sizeof(waveHeader));
	int headerSize = sizeof(waveHeader);


	OperatingInfo oi{ buffer, headerSize };

	return oi;
}

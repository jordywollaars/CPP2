#include "CAFHandler.hpp"

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <regex>

#include "BufferActions.hpp"

std::vector<std::bitset<8>> CAFHandler::readFile() const
{
	//Setup buffer from file content
	const std::vector<char> buffer = this->createBuffer();

	//Search audio data chunk
	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(this->dataPattern), std::end(this->dataPattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("\"data\" could not be found in buffer");
	}

	//Get required information from format headers
	const CAFHandler::AdditionalInfo additionalInfo = this->getAdditionalInfo(buffer);

	//Determine endianess
	if (additionalInfo.flag == 0)
	{
		//Advance iterator to reading start position (skip 17 bytes)
		std::advance(it, 17);
	}
	else
	{
		//Advance iterator to reading start position (skip 16 bytes)
		std::advance(it, 16);
	}

	int steps = additionalInfo.sampleRate / 8;
	auto end = buffer.end();

	//Read buffer for potential message
	const BufferActions bActions{};
	return bActions.readBuffer(steps, it, end);
}

void CAFHandler::writeMessageInFile() const
{
	//Setup buffer from file content
	std::vector<char> buffer = this->createBuffer();

	//Search audio data chunk
	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(this->dataPattern), std::end(this->dataPattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("\"data\" could not be found in buffer");
	}

	//Get required information from format headers
	const CAFHandler::AdditionalInfo additionalInfo = this->getAdditionalInfo(buffer);

	//Determine endianess
	if (additionalInfo.flag == 0)
	{
		//Advance iterator to writing start position (skip 17 bytes)
		std::advance(it, 17);
	}
	else
	{
		//Advance iterator to writing start position (skip 16 bytes)
		std::advance(it, 16);
	}

	int stepSize = additionalInfo.sampleRate / 8;
	auto end = buffer.end();

	//Hide message in buffer content
	const BufferActions bufferActions{};
	bufferActions.writeMessageInBuffer(this->messageBits, stepSize, it, end);

	//Open file to write modified data
	std::ofstream ofs;
	ofs.open(this->operatingPath, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);

	//Write each byte to file
	for (uint8_t byte : buffer)
	{
		char c = byte;
		ofs << c;
	}

	//Close file
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

CAFHandler::AdditionalInfo CAFHandler::getAdditionalInfo(const std::vector<char>& buffer) const
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

	const AdditionalInfo ai{ flag, getSampleSizeFromBuffer(buffer) };
	return ai;
}

std::string CAFHandler::getExpressionString() const
{
	return this->expression;
}

std::string CAFHandler::getExtensionString() const
{
	return this->extension;
}

std::vector<char> CAFHandler::createBuffer() const
{
	//Open file in binary mode
	std::ifstream input(this->operatingPath, std::ios::binary);

	if (!input) //No input found
	{
		throw std::runtime_error("Could not open file");
	}

	//Copy all data into buffer
	std::vector<char> buffer(std::istreambuf_iterator<char>(input), {});

	//Close file
	input.close();

	return buffer;
}

void CAFHandler::setMessageToHide(const std::string& message)
{
	//Clear messageBits vector
	this->messageBits.clear();

	//Put each seperate bit from message into messageBits vector
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

void CAFHandler::setOperatingPath(const std::string& filepath)
{
	const std::regex expression(this->getExpressionString());

	//Check whether filepath is compatible with the file format
	if (!std::regex_match(filepath, expression))
	{
		throw std::invalid_argument("Invalid file extension.");
	}

	//Set operatingPath with filepath
	operatingPath = filepath;
}

std::string CAFHandler::getOperatingPath() const
{
	return this->operatingPath;
}
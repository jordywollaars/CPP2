#include "AIFFHandler.hpp"

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <regex>

#include "BufferActions.hpp"

std::vector<std::bitset<8>> AIFFHandler::readFile() const
{
	//Setup buffer from file content
	const std::vector<char> buffer = this->createBuffer();
	
	//Search audio data chunk
	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(this->ssndPattern), std::end(this->ssndPattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("\"SSND\" could not be found in the file");
	}

	//Advance iterator to reading start position (skip 17 bytes)
	std::advance(it, 17);

	int stepSize = getSampleSizeFromBuffer(buffer) / 8;
	auto end = buffer.end();

	//Read buffer for potential message
	const BufferActions bActions{};
	return bActions.readBuffer(stepSize, it, end);
}

void AIFFHandler::writeMessageInFile() const
{
	//Setup buffer from file
	std::vector<char> buffer = this->createBuffer();

	//Search audio data chunk
	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(this->ssndPattern), std::end(this->ssndPattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("\"SSND\" could not be found in the file");
	}

	//Advance iterator to writing start position (skip 17 bytes)
	std::advance(it, 17);

	int stepSize = getSampleSizeFromBuffer(buffer) / 8;
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

int AIFFHandler::getSampleSizeFromBuffer(const std::vector<char>& buffer) const
{
	auto temp = std::search(std::begin(buffer), std::end(buffer), std::begin(this->commPattern), std::end(this->commPattern));
	if (temp == std::end(buffer))
	{
		throw std::runtime_error("\"COMM\" could not be found in the file");
	}

	std::advance(temp, 14);

	unsigned char byte1 = *temp;
	++temp;
	unsigned char byte2 = *temp;
	int sampleSize = (byte1 << 8) + byte2;

	return sampleSize;
}

std::string AIFFHandler::getExpressionString() const
{
	return this->expression;
}

std::string AIFFHandler::getExtensionString() const
{
	return this->extension;
}

std::vector<char> AIFFHandler::createBuffer() const
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

void AIFFHandler::setMessageToHide(const std::string& message)
{
	//Clear messageBits vector
	this->messageBits.clear();

	//Put each seperate bit from message into messageBits vector
	for (int i = 0; i < message.size(); i++)
	{
		const std::bitset<8> byte(message[i]);

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

void AIFFHandler::setOperatingPath(const std::string& filepath)
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

std::string AIFFHandler::getOperatingPath() const
{
	return this->operatingPath;
}
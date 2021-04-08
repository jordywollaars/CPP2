#include "WaveHandler.hpp"

#include <iostream>
#include <fstream>
#include <ios>
#include <regex>
#include "BufferActions.hpp"

std::vector<std::bitset<8>> WaveHandler::readFile() const
{
	//Setup buffer from file content
	const std::vector<char> buffer = this->createBuffer();

	//Search audio data chunk
	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(this->pattern), std::end(this->pattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("\"data\" could not be found in the file");
	}

	//Advance iterator to reading start position
	std::advance(it, 8);

	int steps = getSampleSizeFromBuffer(buffer) / 8;
	auto end = buffer.end();

	//Read buffer for potential message
	const BufferActions bActions{};
	return bActions.readBuffer(steps, it, end);
}

void WaveHandler::writeMessageInFile() const
{
	//Setup buffer from file content
	std::vector<char> buffer = this->createBuffer();

	//Search audio data chunk
	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(this->pattern), std::end(this->pattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("\"data\" could not be found in the file");
	}

	//Advance iterator to writing start position (skip 8 bytes)
	std::advance(it, 8);

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

int WaveHandler::getSampleSizeFromBuffer(const std::vector<char>& buffer) const
{
	auto it = std::search(std::begin(buffer), std::end(buffer), std::begin(this->pattern), std::end(this->pattern));
	if (it == std::end(buffer))
	{
		throw std::runtime_error("\"data\" could not be found in the file");
	}
	auto temp = std::prev(it, 2);

	unsigned char byte1 = *temp;
	++temp;
	unsigned char byte2 = *temp;
	int sampleSize = (byte2 << 8) + byte1;

	return sampleSize;
}

std::string WaveHandler::getExpressionString() const
{
	return this->expression;
}

std::string WaveHandler::getExtensionString() const
{
	return this->extension;
}

std::vector<char> WaveHandler::createBuffer() const
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

void WaveHandler::setMessageToHide(const std::string& message)
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

void WaveHandler::setOperatingPath(const std::string& filepath)
{
	std::regex expression(this->getExpressionString());

	//Check whether filepath is compatible with the file format
	if (!std::regex_match(filepath, expression))
	{
		throw std::invalid_argument("Invalid file extension.");
	}

	//Set operatingPath with filepath
	operatingPath = filepath;
}

std::string WaveHandler::getOperatingPath() const
{
	return this->operatingPath;
}
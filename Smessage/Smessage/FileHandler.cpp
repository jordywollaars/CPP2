#include "FileHandler.hpp"

#include <fstream>
#include <ios>
#include <regex>

std::vector<char> FileHandler::setupOperatingInfo() const
{
	std::ifstream input(this->operatingPath, std::ios::binary);

	if (!input)
	{
		throw std::runtime_error("Could not open file");
	}

	// copies all data into buffer
	std::vector<char> buffer(std::istreambuf_iterator<char>(input), {});

	return buffer;
}

void FileHandler::setMessageToHide(const std::string& message)
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

void FileHandler::setOperatingPath(const std::string& filepath)
{
	std::regex expression(this->getExpressionString());

	if (!std::regex_match(filepath, expression))
	{
		throw std::invalid_argument("Invalid file extension.");
	}

	operatingPath = filepath;
}

std::string FileHandler::getOperatingPath() const
{
	return this->operatingPath;
}

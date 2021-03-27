#pragma once

#include <string>
#include <vector>
#include <bitset>

class WaveHandler
{
private:
	std::string messageToHide;
	std::vector<bool> messageBits;
public:
	const std::vector<std::bitset<8>> readFile(const std::string filepath);
	int determineHeaderSize(const std::string filepath);
	const void setMessageToHide(std::string message);
	void writeMessageInFile(const std::string filepath);
};
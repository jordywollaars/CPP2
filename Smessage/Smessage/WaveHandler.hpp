#pragma once

#include <string>
#include <vector>
#include <bitset>

#include "OperatingInfo.hpp";

class WaveHandler
{
private:
	std::string messageToHide;
	std::string operatingPath;
	std::vector<bool> messageBits;
public:
	std::vector<std::bitset<8>> readFile() const;
	void writeMessageInFile() const;

	OperatingInfo setupOperatingInfo() const;
	
	std::string getOperatingPath() const;

	void setMessageToHide(const std::string& message);
	void setOperatingPath(const std::string& filepath);
};
#pragma once

#include <string>
#include <vector>
#include <bitset>

#include "OperatingInfo.hpp";

class FileHandler
{
protected:
	std::string messageToHide;
	std::string operatingPath;
	std::vector<bool> messageBits;
public:
	virtual std::vector<std::bitset<8>> readFile() const = 0;
	virtual void writeMessageInFile() const = 0;

	virtual OperatingInfo setupOperatingInfo() const = 0;

	virtual std::string getOperatingPath() const = 0;

	virtual void setMessageToHide(const std::string& message) = 0;
	virtual void setOperatingPath(const std::string& filepath) = 0;
};
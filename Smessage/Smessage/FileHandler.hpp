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

	virtual int getSampleSizeFromBuffer(const std::vector<char>& buffer) const = 0;

	virtual const std::string getExpressionString() const = 0;
	virtual const std::string getExtensionString() const = 0;

	virtual std::string getOperatingPath() const;

	virtual void setMessageToHide(const std::string& message);
	virtual void setOperatingPath(const std::string& filepath);

	virtual std::vector<char> setupOperatingInfo() const;
};
#pragma once

#include <string>
#include <vector>
#include <bitset>

class FileHandler
{
protected:
	std::string messageToHide;
	std::string operatingPath;
	std::vector<bool> messageBits;
public:
	virtual ~FileHandler() {};

	virtual std::vector<std::bitset<8>> readFile() const = 0;
	virtual void writeMessageInFile() const = 0;

	virtual int getSampleSizeFromBuffer(const std::vector<char>& buffer) const = 0;

	virtual std::string getExpressionString() const = 0;
	virtual std::string getExtensionString() const = 0;

	virtual std::string getOperatingPath() const = 0;

	virtual void setMessageToHide(const std::string& message) = 0;
	virtual void setOperatingPath(const std::string& filepath) = 0;

	virtual std::vector<char> createBuffer() const = 0;
};
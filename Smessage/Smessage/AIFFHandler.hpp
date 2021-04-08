#pragma once

#include "FileHandler.hpp"

class AIFFHandler : public FileHandler
{
private:
	const std::vector<int> commPattern{ 0x43, 0x4f, 0x4d, 0x4d };
	const std::vector<int> ssndPattern{ 0x53, 0x53, 0x4e, 0x44 };
public:
	virtual std::vector<std::bitset<8>> readFile() const override;
	virtual void writeMessageInFile() const override;

	virtual int getSampleSizeFromBuffer(const std::vector<char>& buffer) const override;

	virtual const std::string getExpressionString() const override;

	virtual const std::string getExtensionString() const override;
};
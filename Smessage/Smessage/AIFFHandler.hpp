#pragma once

#include "FileHandler.hpp"

class AIFFHandler : public FileHandler
{
private:
	const std::string expression{ "^.*\.(aiff|AIFF|aif|AIF)$" };
	const std::string extension{ "aiff" };
	const std::vector<int> commPattern{ 0x43, 0x4f, 0x4d, 0x4d };
	const std::vector<int> ssndPattern{ 0x53, 0x53, 0x4e, 0x44 };
public:
	virtual std::vector<std::bitset<8>> readFile() const override;
	virtual void writeMessageInFile() const override;

	virtual int getSampleSizeFromBuffer(const std::vector<char>& buffer) const override;

	virtual std::string getExpressionString() const override;

	virtual std::string getExtensionString() const override;

	// Inherited via FileHandler
	virtual std::string getOperatingPath() const override;
	virtual void setMessageToHide(const std::string& message) override;
	virtual void setOperatingPath(const std::string& filepath) override;
	virtual std::vector<char> createBuffer() const override;
};
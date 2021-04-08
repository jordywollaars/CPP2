#pragma once

#include "FileHandler.hpp"

class CAFHandler : public FileHandler
{
private:
	const std::string expression{ "^.*\.(caf|CAF)$" };
	const std::string extension{ "caf" };
	const std::vector<int> dataPattern{ 0x64, 0x61, 0x74, 0x61 };
	const std::vector<int> descPattern{ 0x64, 0x65, 0x73, 0x63 };
public:
	virtual std::vector<std::bitset<8>> readFile() const override;
	virtual void writeMessageInFile() const override;
	virtual int getSampleSizeFromBuffer(const std::vector<char>& buffer) const override;

	struct AdditionalInfo {
		int flag;
		int sampleRate;
	};

	virtual AdditionalInfo getAdditionalInfo(const std::vector<char>& buffer) const;

	virtual std::string getExpressionString() const override;

	virtual std::string getExtensionString() const override;

	virtual std::string getOperatingPath() const override;
	virtual void setMessageToHide(const std::string& message) override;
	virtual void setOperatingPath(const std::string& filepath) override;
	virtual std::vector<char> createBuffer() const override;
};
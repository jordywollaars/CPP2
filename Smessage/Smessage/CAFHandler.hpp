#pragma once

#include "FileHandler.hpp"

class CAFHandler : public FileHandler
{
private:
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

	virtual const AdditionalInfo getAdditionalInfo(const std::vector<char>& buffer) const;

	virtual const std::string getExpressionString() const override;

	virtual const std::string getExtensionString() const override;
};
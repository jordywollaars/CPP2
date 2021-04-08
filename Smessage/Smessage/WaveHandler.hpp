#pragma once

#include "FileHandler.hpp";

class WaveHandler : public FileHandler
{
private:
	const std::vector<int> pattern{ 0x64, 0x61, 0x74, 0x61 };
public:
	std::vector<std::bitset<8>> readFile() const override;
	void writeMessageInFile() const override;

	virtual int getSampleSizeFromBuffer(const std::vector<char>& buffer) const override;
};
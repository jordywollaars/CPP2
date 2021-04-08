#pragma once

#include "FileHandler.hpp";

class WaveHandler : public FileHandler
{
public:
	std::vector<std::bitset<8>> readFile() const override;
	void writeMessageInFile() const override;
};
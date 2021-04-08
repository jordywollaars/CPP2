#pragma once

#include "FileHandler.hpp"

class AIFFHandler : public FileHandler
{
	// Inherited via FileHandler
	virtual std::vector<std::bitset<8>> readFile() const override;
	virtual void writeMessageInFile() const override;
};
#pragma once

#include "FileHandler.hpp";

class WaveHandler : public FileHandler
{
public:
	std::vector<std::bitset<8>> readFile() const override;
	void writeMessageInFile() const override;

	OperatingInfo setupOperatingInfo() const override;
	
	std::string getOperatingPath() const override;

	void setMessageToHide(const std::string& message) override;
	void setOperatingPath(const std::string& filepath) override;
};
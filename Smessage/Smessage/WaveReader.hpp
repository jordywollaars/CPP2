#pragma once

#include <string>
#include <vector>
#include <bitset>

class WaveReader
{
public:
	std::vector<std::bitset<8>> readFile(const std::string filepath);
	int determineHeaderSize(const std::string filepath);
};
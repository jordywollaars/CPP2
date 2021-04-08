#pragma once
#include <vector>
#include <bitset>

class BufferActions
{
public:
	virtual std::vector<std::bitset<8>> readBuffer(int stepSize, std::vector<char>::const_iterator& start, std::vector<char>::const_iterator& end) const;
	virtual void writeMessageInBuffer(const std::vector<bool>& messageBits, int stepSize, std::vector<char>::iterator& start, std::vector<char>::const_iterator& end) const;
};
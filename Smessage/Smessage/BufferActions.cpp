#include "BufferActions.hpp"

#include <stdexcept>

std::vector<std::bitset<8>> BufferActions::readBuffer(int stepSize, std::vector<char>::const_iterator& start, std::vector<char>::const_iterator& end) const
{
	std::vector<std::bitset<8>> possibleMessage;
	std::vector<std::bitset<8>> bits;

	for (auto it = start; it != end; std::advance(it, stepSize))
	{
		//Isolate least significant bit to the left
		std::bitset<8> x(*it);
		x = x << 7;

		//Push bit in bits collection
		bits.push_back(x);

		if (bits.size() == 8)
		{
			//Shift isolated bits to their correct position, from most to least significant.
			for (int i = 0; i < 8; i++)
			{
				std::bitset<8> x(bits[i]);
				std::bitset<8> y = x >> i;

				bits[i] = y;
			}

			//Merge all bits into a byte and push the byte into the possibleMessage vector
			possibleMessage.push_back(
				bits[0] | bits[1] | bits[2] | bits[3] | bits[4] | bits[5] | bits[6] | bits[7]
			);

			//If byte is null message could be found and stop looking
			std::bitset<8> x(possibleMessage[possibleMessage.size() - 1]);
			if (x == 0)
			{
				break;
			}

			//Clear bits vector for creating/finding new byte
			bits.clear();
		}
	}

	//Return potential message
	return possibleMessage;
}

void BufferActions::writeMessageInBuffer(const std::vector<bool>& messageBits, int stepSize, std::vector<char>::iterator& start, std::vector<char>::const_iterator& end) const
{
	uint8_t mask1{ 1 }; // Mask 00000001
	uint8_t mask2{ 0xFE }; // Mask 11111110

	int counter = 0;
	int nullByteCounter = 8;

	for (auto& it = start; it != end; std::advance(it, stepSize))
	{
		if (counter < messageBits.size()) //There are more bits to hide
		{
			if (*it & mask1) //usable bit is 1
			{
				if (messageBits[counter] != 1) //If usable bit is not the same as current message bit
				{
					*it = *it & mask2; //Flip bit value
				}
			}
			else //usable bit is 0
			{
				if (messageBits[counter] != 0) //If usable bit is not the same as current message bit
				{
					*it = *it | mask1; //Flip bit value
				}
			}

			counter++;
		}
		else if (nullByteCounter >= 0) //Message is hidden and null byte can now be appended
		{
			if (*it & mask1) //usable bit is 1
			{
				*it = *it & mask2; //Flip bit value
			}

			std::bitset<8> x(*it);
			//std::cout << x << " - 0" << std::endl;

			nullByteCounter--;
		}
		else //Completed hiding message with closing null byte
		{
			break;
		}
	}

	//If counter is lower than messageBits vector size AND nullByteCounter hasn't finished
	if (counter < messageBits.size() && nullByteCounter >= 0)
	{
		throw std::runtime_error("Amount of message bits exceed buffer size.");
	}
}

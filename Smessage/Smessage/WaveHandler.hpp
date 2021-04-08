#pragma once

#include "FileHandler.hpp";

class WaveHandler : public FileHandler
{
private:
	const std::string expression{ "^.*\.(wav|WAV)$" };
	const std::string extension{ "wav" };
	const std::vector<int> pattern{ 0x64, 0x61, 0x74, 0x61 };
public:
	std::vector<std::bitset<8>> readFile() const override;
	void writeMessageInFile() const override;

	virtual int getSampleSizeFromBuffer(const std::vector<char>& buffer) const override;

	virtual std::string getExpressionString() const override;

	virtual std::string getExtensionString() const override;

	virtual std::string getOperatingPath() const override;
	virtual void setMessageToHide(const std::string& message) override;
	virtual void setOperatingPath(const std::string& filepath) override;
	virtual std::vector<char> createBuffer() const override;
};
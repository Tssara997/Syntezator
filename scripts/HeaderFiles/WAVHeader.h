#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>

struct WAVHeader {
	char riff[4] = { 'R', 'I', 'F', 'F' };
	uint32_t chunkSize;
	char wav[4] = { 'W', 'A', 'V', 'E' };
	char fmt[4] = { 'f', 'm', 't', ' ' };
	uint32_t subchunkSize = 16;
	uint16_t audioFormat = 1; // PCM
	uint16_t numChannels = 1; // mono
	uint32_t sampleRate = 44100;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample = 16;
	char data[4] = { 'd', 'a', 't', 'a' };
	uint32_t dataSize;
};
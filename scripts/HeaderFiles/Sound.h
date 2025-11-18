#pragma once

#include <xaudio2.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#pragma comment(lib, "xaudio2.lib")

struct SoundData {
	WAVEFORMATEX wfx;
	std::vector<BYTE> buffer;
};

struct Voice {
	SoundData wav;
	IXAudio2SourceVoice* voice{};
	XAUDIO2_BUFFER buf = { 0 };
	std::string path = "";
};

class Sound {
private:

protected:
	IXAudio2* xaudio;
	IXAudio2MasteringVoice* master;
	std::vector<Voice> voices;

public:
	Sound();
	~Sound();
	SoundData fromWaveFile(const std::string& path);
	SoundData fromCsvFile(const std::string& path);
	void playSound(const Voice& voice) const;
	void deleteVoice(Voice& voice);
	void createVoice(Voice& voice, const std::string& path);
};


#pragma once

#include <xaudio2.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <map>

#pragma comment(lib, "xaudio2.lib")

struct SoundData {
	WAVEFORMATEX wfx{};
	std::vector<BYTE> buffer;
};

struct VoiceData {
	SoundData wav;
	IXAudio2SourceVoice* voice{};
	XAUDIO2_BUFFER buf = { 0 };
	std::string path = "";
};

enum class Voices {
	EMPTY = 0,
	METRONOM_SLABY,
	METRONOM_GLOSNY,
	OSCYLATOR_A,
	OSYLATOR_B
};

class Sound {
private:
	std::map<Voices, std::string> voicePaths{
		{Voices::EMPTY, "" },
		{ Voices::METRONOM_SLABY, "sounds\\barr.wav" },
		{ Voices::METRONOM_GLOSNY, "sounds\\bitt.wav" },
		{ Voices::OSCYLATOR_A, "dane\\dane.csv" }
		//{ Voices::OSYLATOR_B, "sounds\\oscylator_b.wav" }
};

protected:
	IXAudio2* xaudio;
	IXAudio2MasteringVoice* master;
	std::vector<VoiceData> voiceVector;
	double frequencyRatio;
	double volumeRatio;

public:
	Sound();
	~Sound();
	SoundData fromWaveFile(const std::string& path);
	SoundData fromCsvFile(const std::string& path);
	void playSound(const enum Voices& dzwiek);
	void deleteVoice(VoiceData& voice);
	void createVoice(const std::string& path, enum Voices );
	void stopSound(const enum Voices& dzwiek);
	void setfrequencyRatio(const enum Voices& dzwiek, const double& ratio);
	void setVolumeRatio(const enum Voices& dzwiek, const double& ratio);
	void SoundInit();

};


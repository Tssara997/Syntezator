#include "HeaderFiles/Sound.h"

Sound::Sound() : xaudio{nullptr}, master{nullptr}
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	
	if (FAILED(XAudio2Create(&xaudio, 0))) {
		std::cerr << "XAudioCreate Failed" << std::endl;
	}

	xaudio->CreateMasteringVoice(&master);

	for (int i{}; i < static_cast<int>(Voices::OSYLATOR_B) + 1; i++)
		voiceVector.push_back(VoiceData{});

	createVoice("sounds\\bitt.wav", Voices::METRONOM_SLABY);
	createVoice("sounds\\barr.wav", Voices::METRONOM_GLOSNY);
	createVoice("dane\\dane.csv", Voices::OSCYLATOR_A);
}

Sound::~Sound()
{
	for (auto& v : voiceVector) {
		if (v.voice != nullptr)
			v.voice->DestroyVoice();
	}
	if (master != nullptr) {
		master->DestroyVoice();
	}
	xaudio->Release();
	CoUninitialize();
}

void Sound::playSound(const enum Voices& dzwiek) {
	size_t i = (size_t)dzwiek;
	voiceVector.at(i).voice->Stop();
	if (voiceVector.at(i).voice != nullptr)
		voiceVector.at(i).voice->FlushSourceBuffers();
	voiceVector.at(i).buf.AudioBytes = (UINT32)voiceVector.at(i).wav.buffer.size();
	voiceVector.at(i).buf.pAudioData = voiceVector.at(i).wav.buffer.data();
	if (dzwiek == Voices::OSCYLATOR_A || dzwiek == Voices::OSYLATOR_B) {
		voiceVector.at(i).buf.LoopBegin = 0;
		voiceVector.at(i).buf.LoopLength = voiceVector.at(i).buf.PlayLength;
		voiceVector.at(i).buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	voiceVector.at(i).buf.Flags = XAUDIO2_END_OF_STREAM;
	voiceVector.at(i).voice->SubmitSourceBuffer(&voiceVector.at(i).buf);
	voiceVector.at(i).voice->Start();
}

void Sound::stopSound(const enum Voices& dzwiek) {
	size_t i = (size_t)dzwiek;
	voiceVector.at(i).voice->Stop();
	if (voiceVector.at(i).voice != nullptr)
		voiceVector.at(i).voice->FlushSourceBuffers();
}

void Sound::deleteVoice(VoiceData& voice)
{
	for (size_t i{}; i < voiceVector.size(); i++) {
		if (voiceVector[i].path == voice.path) {
			voiceVector[i].voice->DestroyVoice();
			voiceVector.erase(voiceVector.begin() + i);
			break;
		}
	}
}

void Sound::createVoice(const std::string& path, enum Voices dzwiek) {
	size_t i = (size_t)dzwiek;
	VoiceData voice;
	voice.path = path;
	if (path[path.size() - 3] == 'c' ) {
		voice.wav = fromCsvFile(voice.path);
	}
	else
		voice.wav = fromWaveFile(voice.path);
	xaudio->CreateSourceVoice(&voice.voice, &voice.wav.wfx, 0, 1.0, NULL, NULL, NULL);
	voiceVector.at(i) = voice;

}

SoundData Sound::fromWaveFile(const std::string& path)
{
	SoundData s;

	std::ifstream f(path, std::ios::binary);

	if (!f) {
		std::cerr << "Nie mozna otworzyc pliku wav" << std::endl;
		return s;
	}

	char chunkId[4];
	DWORD dwChunkSize;
	std::string test;

	// finding the RIFF
	while (true) {
		f.read(chunkId, 4);
		f.read((char*)&dwChunkSize, 4);
		if (strncmp(chunkId, "RIFF", 4) == 0) {
			break;
		}
		if (dwChunkSize % 2 == 1) f.seekg(1, std::ios::cur);
	}
	f.seekg(8, std::ios::cur);

	// Locating the fmt chunk and copy its contents into WAVEFORMATEX structre
	while (true) {
		f.read(chunkId, 4);
		f.read((char*)&dwChunkSize, 4);
		if (strncmp(chunkId, "fmt ", 4) == 0) { break; }

		if (dwChunkSize % 2 == 1) f.seekg(1, std::ios::cur);

	}

	ZeroMemory(&s.wfx, sizeof(WAVEFORMATEX));

	f.read((char*)&s.wfx.wFormatTag, 2);
	f.read((char*)&s.wfx.nChannels, 2);
	f.read((char*)&s.wfx.nSamplesPerSec, 4);
	f.read((char*)&s.wfx.nAvgBytesPerSec, 4);
	f.read((char*)&s.wfx.nBlockAlign, 2);
	f.read((char*)&s.wfx.wBitsPerSample, 2);
	s.wfx.cbSize = 0;

	if (dwChunkSize > 16)
		f.seekg(dwChunkSize - 16, std::ios::cur);

	while (true) {
		f.read(chunkId, 4);
		f.read((char*)&dwChunkSize, 4);
		if (strncmp(chunkId, "data", 4) == 0) {
			break;
		}
		if (dwChunkSize % 2 == 1) f.seekg(1, std::ios::cur);
	}

	s.buffer.resize(dwChunkSize);
	f.read((char*)s.buffer.data(), dwChunkSize);

	return s;
}

SoundData Sound::fromCsvFile(const std::string& path)
{
	SoundData s;

	s.wfx.wFormatTag = WAVE_FORMAT_PCM;
	s.wfx.nChannels = 1;
	s.wfx.nSamplesPerSec = 44100;
	s.wfx.wBitsPerSample = 16;
	s.wfx.nBlockAlign = s.wfx.nChannels * s.wfx.wBitsPerSample / 8;
	s.wfx.nAvgBytesPerSec = s.wfx.nSamplesPerSec * s.wfx.nBlockAlign;
	s.wfx.cbSize = 0;

	std::ifstream f(path);
	if (!f) {
		std::cerr << "Nie mozna odtworzyc pliku csv do stworzenia SoundData" << std::endl;
		return s;
	}
	double value;
	while (f >> value) {
		short pcm = (short)(value * 32767.0f);
		BYTE* p = reinterpret_cast<BYTE*>(&pcm);
		s.buffer.push_back(p[0]);
		s.buffer.push_back(p[1]);
	}

	return s;
}

#include <xaudio2.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#pragma comment(lib, "xaudio2.lib")


struct SoundData {
	WAVEFORMATEX wfx = {0};
	std::vector<BYTE> buffer = {0};
};

SoundData FromWavFile(const std::string& path);

SoundData FromCsvFile(const std::string& path);

int main() {
	// inicjalizacja XAudio2

	IXAudio2* xaudio = nullptr;
	HRESULT hrT = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	std::cout << "CoInitializeEx hr=0x" << std::hex << hrT << std::dec << "\n";
	if (FAILED(XAudio2Create(&xaudio, 0))) {
		std::cerr << "XAudioCreate Failed" << std::endl;
		return -1;
	}

	IXAudio2MasteringVoice* master = nullptr;
	xaudio->CreateMasteringVoice(&master);

	// wczytujemy probki WAV
	SoundData wav1 = FromWavFile("sounds\\barr.wav");
	SoundData wav2 = FromWavFile("sounds\\bitt.wav");
	// trowrzymy source voice
	IXAudio2SourceVoice* voice1{};
	IXAudio2SourceVoice* voice2{};

	std::cout << wav1.wfx.wFormatTag << ' ' << wav1.wfx.wBitsPerSample << ' ' << wav1.wfx.cbSize;

	HRESULT hr = xaudio->CreateSourceVoice(&voice1, &wav1.wfx, 0, 1.0, NULL, NULL, NULL);
	xaudio->CreateSourceVoice(&voice2, &wav2.wfx, 0, 1.0);

	if (FAILED(hr)) {
		std::cerr << "CreateSourceVoice failed: 0x"
			<< std::hex << hr << std::dec << std::endl;
		std::cout << "wav1.buffer.size(): " << wav1.buffer.size() << std::endl;

		return -1;
	}
	// budujemy bufory
	XAUDIO2_BUFFER buf = { 0 };
	buf.pAudioData = wav1.buffer.data();
	buf.AudioBytes = (UINT32)wav1.buffer.size();
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//voice1->Stop();      // reset voice
	//voice1->FlushSourceBuffers();
	voice1->SubmitSourceBuffer(&buf);
	voice1->Start();
	// cleanup
	//Sleep(3000);
	XAUDIO2_BUFFER buf1 = { 0 };
	buf1.pAudioData = wav1.buffer.data();
	buf1.AudioBytes = (UINT32)wav1.buffer.size();
	buf1.Flags = XAUDIO2_END_OF_STREAM;

	voice1->Stop();      // reset voice
	voice1->FlushSourceBuffers();
	voice1->SubmitSourceBuffer(&buf1);
	voice1->Start();



	voice1->DestroyVoice();
	voice2->DestroyVoice();
	master->DestroyVoice();
	xaudio->Release();

	CoUninitialize();

	return 0;
}

SoundData FromWavFile(const std::string& path) {
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

SoundData FromCsvFile(const std::string& path) {
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
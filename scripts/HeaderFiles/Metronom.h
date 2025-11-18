#pragma once

#pragma comment(lib, "winmm.lib")

#include <chrono>
#include <Windows.h>
#include <iostream>
#include <x3daudio.h>

#pragma comment(lib, "xaudio2.lib")

#include "SoundNotLoadingExepction.h"
#include "Sound.h"

class Metronom {
private:
	static constexpr int beatsNum = 4;
	static constexpr int miliSecunds = 60000;
	static constexpr int defTempo = 140;
	static constexpr bool defAktywny = false;
protected:
	int tempo;
	double czasInterwalu;
	int numerInterwalu;
	std::chrono::steady_clock::time_point start;
	Voice metronomMocny;
	Voice metronomSlaby;
	Sound sound;

	bool zobaczCzyInterwal();
	void obliczanieTrwaniaTempa();
	void playSound();
public:
	Metronom(Sound& sound, int tempo = defTempo);
	~Metronom();
	void play();
	int getTempo() const;
	void changeTempo(int tempo);
	void quit();
};
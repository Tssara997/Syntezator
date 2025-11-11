#pragma once

#pragma comment(lib, "winmm.lib")

#include <chrono>
#include <Windows.h>
#include <iostream>

#include "SoundNotLoadingExepction.h"

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

	bool zobaczCzyInterwal();
	void obliczanieTrwaniaTempa();
	const void playSound() const;
public:
	Metronom(int tempo = defTempo);
	~Metronom() = default;
	void play();
	int getTempo() const;
	void changeTempo(int tempo);
	void quit();
};
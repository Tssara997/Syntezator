#pragma once

#pragma comment(lib, "winmm.lib")

#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include <x3daudio.h>

#pragma comment(lib, "xaudio2.lib")
#include "WAVHeader.h"
#include "Sound.h"

class Fala
{
	// czespr / czes
private:
	static constexpr double defCzestotliwosc = 100;
	static constexpr double defCzestotliwoscProbkowania = 44100.0;
	static constexpr double defGlosnosc = 100;
	static constexpr double defAmplituda = 0.5;
	static constexpr double defprzesunieceFazy = 0;
	static constexpr int defChannels = 1;
	static constexpr double czestotliwoscRysowania = 0.0001;
	static constexpr bool defActiveSound = false;

protected:
	double czestotliwosc;
	double czestotliwoscProbkowania;
	double glosnosc;
	double amplituda;
	double przesuniecieFazy;
	int channels;
	bool activeSound;

public:
	Fala() : czestotliwosc{ defCzestotliwosc }, czestotliwoscProbkowania{ defCzestotliwoscProbkowania }, glosnosc{ defGlosnosc }, amplituda{ defAmplituda }, przesuniecieFazy{ defprzesunieceFazy }, channels{ defChannels }, activeSound{ defActiveSound } {}
	~Fala() = default;
	bool create() {}
	void draw() const;
	void zmienFaze(double x);
	void stworzWav();
	double getFaza() const;
	enum Voices grajDzwiek();
	bool isActive() const { return activeSound; }
	void setActive(const bool status) { activeSound = status; }
};


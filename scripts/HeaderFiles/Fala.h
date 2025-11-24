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
	static constexpr double defCzestotliwosc = 265;
	static constexpr double defCzestotliwoscProbkowania = 44100.0;
	static constexpr double defGlosnosc = 100;
	static constexpr double defAmplituda = 1;
	static constexpr double defprzesunieceFazy = 0;
	static constexpr int defChannels = 1;
	static constexpr double czestotliwoscRysowania = 0.0001;
	static constexpr bool defActiveSound = false;
	static constexpr double defOkres = 1;

protected:
	double czestotliwosc;
	double czestotliwoscProbkowania;
	double glosnosc;
	double amplituda;
	double przesuniecieFazy;
	int channels;
	bool activeSound;
	double okres;

public:
	Fala() : czestotliwosc{ defCzestotliwosc }, czestotliwoscProbkowania{ defCzestotliwoscProbkowania }, okres{ defOkres }, glosnosc{ defGlosnosc }, 
		amplituda{ defAmplituda }, przesuniecieFazy{ defprzesunieceFazy }, channels{ defChannels }, activeSound{ defActiveSound } {}
	~Fala() = default;
	bool create() {}
	void draw() const;

	void zmienFaze(double x);
	double getFaza() const;
	void zmienAmplitude(double x);
	double getAmplituda() const;
	void zmienCzestotliwosc(double x) { czestotliwosc = x; }
	double getCzestotliwosc() const { return czestotliwosc; }

	enum Voices grajDzwiek();
	bool isActive() const { return activeSound; }
	void setActive(const bool status) { activeSound = status; }
	void stworzWav();
};


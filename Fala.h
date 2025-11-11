#pragma once

#pragma comment(lib, "winmm.lib")

#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include "WAVHeader.h"


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

protected:
	double czestotliwosc;
	double czestotliwoscProbkowania;
	double glosnosc;
	double amplituda;
	double przesuniecieFazy;
	int channels;

public:
	Fala() : czestotliwosc{ defCzestotliwosc }, czestotliwoscProbkowania{ defCzestotliwoscProbkowania }, glosnosc{ defGlosnosc }, amplituda{ defAmplituda }, przesuniecieFazy{ defprzesunieceFazy }, channels{ defChannels } {}
	~Fala() = default;
	bool create() {}
	void draw() const;
	void zmienFaze(double x);
	double getFaza() const;
	void grajDzwiek();
};


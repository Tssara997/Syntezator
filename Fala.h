#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>

class Fala
{
private:
	static constexpr double defCzestotliwosc = 2;
	static constexpr double defCzestotliwoscProbkowania = 2;
	static constexpr double defGlosnosc = 100;
	static constexpr double defAmplituda = 1;
	static constexpr double defprzesunieceFazy = 0;

protected:
	double czestotliwosc;
	double czestotliwoscProbkowania;
	double glosnosc;
	double amplituda;
	double przesuniecieFazy;

public:
	Fala() : czestotliwosc{ defCzestotliwosc }, czestotliwoscProbkowania{ defCzestotliwoscProbkowania }, glosnosc{ defGlosnosc }, amplituda{ defAmplituda }, przesuniecieFazy{ defprzesunieceFazy } {}
	~Fala() = default;
	bool create() {}
	const std::string draw() const;
};


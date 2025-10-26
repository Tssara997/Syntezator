#pragma comment(lib, "winmm.lib")

#include <chrono>
#include <iostream>
#include <Windows.h>

#include "SoundNotLoadingExepction.h"

// cosnt values
static constexpr int beatsNum = 4;
static constexpr int miliSecunds = 60000;

// global values
int tempo{}; 

// declaration
bool zobaczCzyInterwal(const double& czasInterwalu, std::chrono::steady_clock::time_point& start, int& numerInterwalu);
double obliczanieTrwaniaTempa();
const void playSound(const int& numerInterwalu);

int main() {
	std::cout << "Podaj tempo: ";
	std::cin >> tempo;
	int numerInterwalu{};
	static auto start = std::chrono::steady_clock::now();
	double interwal = obliczanieTrwaniaTempa();
	while (true)
		if (zobaczCzyInterwal(interwal, start, numerInterwalu)) {
			playSound(numerInterwalu);
			try {
				playSound(numerInterwalu);
			}
			catch (const SoundNotLoadingExepction& ex) {
				std::cerr << ex.what() << std::endl;
				return 0;
			}
		}
			
	return 0;
}

double obliczanieTrwaniaTempa() {
	return  (miliSecunds) / static_cast<double>(tempo);
}

bool zobaczCzyInterwal(const double& czasInterwalu, std::chrono::steady_clock::time_point& start, int& numerInterwalu) {
	auto now = std::chrono::steady_clock::now();

	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >= czasInterwalu) {
		start = now;
		numerInterwalu++;
		numerInterwalu %= 4;
		return true;
	}
	return false;
}

const void playSound(const int& numerInterwalu) {
	if (!PlaySound(TEXT("sounds\\barr.wav"), NULL, SND_FILENAME | SND_ASYNC) && 
		!PlaySound(TEXT("sounds\\bitt.wav"), NULL, SND_FILENAME | SND_ASYNC))
		throw SoundNotLoadingExepction();
	if (numerInterwalu != 0) {
		PlaySound(
			TEXT("sounds\\bitt.wav"),
			NULL,
			SND_FILENAME | SND_ASYNC
		);
	}
	else {
		PlaySound(
			TEXT("sounds\\barr.wav"),
			NULL,
			SND_FILENAME | SND_ASYNC
		);
	}
}
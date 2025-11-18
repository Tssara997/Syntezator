#include "HeaderFiles/Metronom.h"

Metronom::Metronom(int tempo) : tempo{ tempo }, czasInterwalu{}, numerInterwalu{}, start{ std::chrono::steady_clock::now() } {
	obliczanieTrwaniaTempa();
}

void Metronom::obliczanieTrwaniaTempa() {
	czasInterwalu = miliSecunds / static_cast<double>(tempo);
}

bool Metronom::zobaczCzyInterwal() {
	auto now = std::chrono::steady_clock::now();

	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >= czasInterwalu) {
		start = now;
		numerInterwalu++;
		numerInterwalu %= beatsNum;
		return true;
	}
	return false;
}

void Metronom::playSound() {
	return;
}

enum Voices Metronom::play() {
	if (zobaczCzyInterwal()) {
		if (numerInterwalu == 0) {
			return Voices::METRONOM_GLOSNY;
		}
		else {
			return Voices::METRONOM_SLABY;
		}
	}
	return Voices::EMPTY;
}

void Metronom::quit() {
	this->numerInterwalu = 0;
}

int Metronom::getTempo() const {
	return tempo;
}

void Metronom::changeTempo(int tempo) {
	this->tempo = tempo;
	obliczanieTrwaniaTempa();
}
#include "HeaderFiles/Metronom.h"

Metronom::Metronom(Sound& sound, int tempo) : tempo{ tempo }, czasInterwalu{}, numerInterwalu{}, start{ std::chrono::steady_clock::now() }, sound{ sound } {
	obliczanieTrwaniaTempa();
	this->sound.createVoice(metronomMocny, "sounds\\barr.wav");
	this->sound.createVoice(metronomSlaby, "sounds\\bitt.wav");
}

Metronom::~Metronom()
{
	sound.deleteVoice(metronomMocny);
	sound.deleteVoice(metronomSlaby);
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
	if (numerInterwalu != 0) {  // granie slabego beata
		sound.playSound(metronomSlaby);
	}
	else { // granie mocnego beata
		sound.playSound(metronomMocny);
	}
}

void Metronom::play() {
	if (zobaczCzyInterwal()) {
		playSound();
	}
}

void Metronom::quit() {
	this->numerInterwalu = 0;
}

int Metronom::getTempo() const {
	return tempo;
}

void Metronom::changeTempo(int tempo) {
	this->tempo = tempo;
	this->numerInterwalu = 0;
	obliczanieTrwaniaTempa();
}
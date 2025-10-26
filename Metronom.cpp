#include "Metronom.h"

Metronom::Metronom(int tempo) : tempo{ tempo }, czasInterwalu{}, numerInterwalu{}, aktywny{ defAktywny }, start{ std::chrono::steady_clock::now() } {
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

const void Metronom::playSound() const {
	if (!PlaySound(TEXT("sounds\\barr.wav"), NULL, SND_FILENAME | SND_ASYNC) || 
		!PlaySound(TEXT("sounds\\bitt.wav"), NULL, SND_FILENAME | SND_ASYNC))
		throw SoundNotLoadingExepction();
	if (numerInterwalu != 0) {  // granie slabego beata
		PlaySound(
			TEXT("sounds\\bitt.wav"),
			NULL,
			SND_FILENAME | SND_ASYNC
		);
	}
	else { // granie mocnego beata
		PlaySound(
			TEXT("sounds\\barr.wav"),
			NULL,
			SND_FILENAME | SND_ASYNC
		);
	}
}

void Metronom::play() {
	aktywny = true;
	while (aktywny) {
		if (zobaczCzyInterwal()) {
			try {
				playSound();
			}
			catch(const SoundNotLoadingExepction& ex) {
				std::cerr << ex.what() << std::endl;
			}
		}
	}
}

void Metronom::quit() {
	aktywny = false;
}
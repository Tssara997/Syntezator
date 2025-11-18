#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <chrono>
#include <thread>
#include <atomic>

#include "HeaderFiles/Fala.h"
#include "HeaderFiles/Metronom.h"

std::atomic<bool> grajMetronomBool = false;
std::atomic<bool> koniecProgramu = false;
std::atomic<bool> grajSinus = false;

void zagrajMetronom(std::shared_ptr<Metronom> metronom);
void zagrajDzwiek(std::shared_ptr<Fala> sinus);
void cleanup();

int main2() {
	// ustawianie serwera i socket dla komunikacji z pythonem
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "Nie mozna zainicjowac WinSock\n";
		return 1;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) {
		std::cerr << "Blad socket(): " << WSAGetLastError() << std::endl;
		return 1;
	}

	sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(12345);

	bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	listen(serverSocket, 1);

	// uruchamianie pythona
	std::cout << "Uruchamianie klienta Python...\n";
	std::system("start /B python scripts\\GUI\\rysowanie.py"); // Windows: /B = bez nowego okna
	std::this_thread::sleep_for(std::chrono::seconds(1)); // czas by python zdazyl sie polaczyc

	std::cout << "Serwer c++ czeka na polaczenie...\n";
	SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
	std::cout << "Polaczono z klientem\n";

	Sound sound;
	std::shared_ptr<Fala> sinus = std::make_shared<Fala>();
	sinus->draw();

	std::ifstream file("dane\\wykres.csv");
	std::string content((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	send(clientSocket, content.c_str(), content.size(), 0);

	std::shared_ptr metronom = std::make_unique<Metronom>(sound);
	std::thread metronomThread(zagrajMetronom, metronom);
	std::thread sinusThread(zagrajDzwiek, sinus);

	// odbieranie danych z Pythona
	char buffer[1024] = {0};
	std::string buf;
	while (true) {
		int valread = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (valread <= 0) {
			// klient siê rozlaczyl lub blad
			break;
		}

		//buffer[valread] = '\0'; // zakoñcz string, bo recv nie dodaje '\0'

		size_t pos;
		buf.append(buffer, valread);
		while ((pos = buf.find('\n')) != std::string::npos) {
			std::string line = buf.substr(0, pos);
			buf.erase(0, pos + 1);

			std::istringstream iss(line);
			std::string cmd;
			double value;
			iss >> cmd >> value;

			if (cmd == "FAZA") {
				sinus->zmienFaze(sinus->getFaza() + value);
				sinus->draw();
				std::ifstream file("dane\\wykres.csv");
				std::string content((std::istreambuf_iterator<char>(file)),
					std::istreambuf_iterator<char>());

				send(clientSocket, content.c_str(), content.size(), 0);
				std::cout << "Zmiana przesuniecia fazy o: " << value << std::endl;
			}
			else if (cmd == "FALA") {
				//sinus->grajDzwiek();
				std::cout << "Muzyka" << std::endl;
			}
			else if (cmd == "METRO") {
				grajSinus = !grajSinus;
				std::cout << "Zmiana tempa metronomu o " << value << std::endl;
			}
			else if (cmd == "METROON") {
				grajMetronomBool = !grajMetronomBool;
				std::cout << "Metronom input" << std::endl;
			}
		}

		std::cout << "Otrzymano z Pythona: " << buffer << std::endl;
		if (buffer)

		memset(buffer, 0, sizeof(buffer)); // wyczyœæ bufor
	}

	koniecProgramu = true;
	metronomThread.join();
	sinusThread.join();

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();

	return 0;
}

void zagrajMetronom(std::shared_ptr<Metronom> metronom) {
	while (!koniecProgramu) {
		if (grajMetronomBool) {
			metronom->play();
		}
		else {
			metronom->quit();
		}
	}
}

void zagrajDzwiek(std::shared_ptr<Fala> s) {
	//while (!koniecProgramu) {
	//	//s->stworzWav();
	//	while (grajSinus) {
	//		//s->grajDzwiek();
	//	}
	//}
	return;
}

void cleanup() {

}

/*if (cmd == "AMP") {
				sinus.zmienAmplitude(sinus.getAmplituda() + value);
				sinus.draw();
				std::ifstream file("dane.csv");
				std::string content((std::istreambuf_iterator<char>(file)),
					std::istreambuf_iterator<char>());

				send(clientSocket, content.c_str(), content.size(), 0);
				std::cout << "Zmiana amplitudy o: " << value << std::endl;
			}*/
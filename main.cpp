#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <chrono>
#include <thread>

#include "Fala.h"

int main() {
	// ustawianie serwera i socket dla komunikacji z pythonem
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(12345);

	bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	listen(serverSocket, 1);

	// uruchamianie pythona
	std::cout << "Uruchamianie klienta Python...\n";
	std::system("start /B python rysowanie.py"); // Windows: /B = bez nowego okna
	std::this_thread::sleep_for(std::chrono::seconds(1)); // czas by python zdazyl sie polaczyc

	std::cout << "Serwer c++ czeka na polaczenie...\n";
	SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
	std::cout << "Polaczono z klientem\n";

	//char buffer[256];
	//int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
	//buffer[bytesReceived] = '\0';
	//std::string waveType = buffer;
	//std::cout << "zadanie fali: " << waveType << std::endl;

	Fala sinus{};

	std::string data = sinus.draw();
	send(clientSocket, data.c_str(), data.size(), 0);

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();

	return 0;
}
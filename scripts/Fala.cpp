#include "HeaderFiles/Fala.h"
void Fala::draw() const
{

    // try
   /* if (!file.is_open()) {
        std::cerr << "Nie otworzono pliku\n";
        return 1;
    }*/

    std::ofstream file("dane\\dane.csv");

    file.clear();

    for (double t = 0; t < okres ; t += 1/czestotliwoscProbkowania) {
        double y =  amplituda * sin(2 * M_PI * czestotliwosc * t  + przesuniecieFazy);
        file << y << "\n";
    }

    file.close();
    std::ofstream filePy("dane\\wykres.csv");

	filePy.clear();

     //sin
    for (double x = 0; x < 1 / czestotliwosc + czestotliwoscRysowania; x += czestotliwoscRysowania) {
        double y = amplituda * sin(2 * M_PI * czestotliwosc * x + przesuniecieFazy);
        filePy<< y << "\n";
    }



    // cos

    //for (double x = 0; x < 1 / czestotliwosc + czestotliwoscRysowania; x += czestotliwoscRysowania) {
    //    double y = amplituda * cos(2 * M_PI * czestotliwosc * x + przesuniecieFazy);
    //    filePy<< y << "\n";
    //}

    // trojkat
    //double x = (1 / czestotliwosc + 2*czestotliwoscRysowania)/2;

    //for (double t = -x; t <= x; t += czestotliwoscRysowania) {
    //    double y =  max(amplituda- abs(t), 0);
    //    filePy<< y << "\n";
    //}

    
    //int x = okres / 2;

    //for (double t = -x; t <= x; t += 1/czestotliwoscProbkowania) {
    //    double y = max(amplituda - abs(t), 0);
    //    filePy << y << "\n";
    //}
    filePy.close();

}

void Fala::zmienFaze(double x)
{
    przesuniecieFazy = x;
}

double Fala::getFaza() const
{
    return przesuniecieFazy;
}

void Fala::zmienAmplitude(double x)
{
    amplituda = x;
}

double Fala::getAmplituda() const
{
    return amplituda;
}

void Fala::stworzWav() {

    //std::string csvFile = "dane\\dane.csv";
    //std::string wavFile = "dane\\output.wav";
    //std::ifstream in(csvFile);
    //if (!in.is_open()) {
    //    std::cerr << "Nie mozna otworzyc pliku csv\n";
    //    return;
    //}

    //std::vector<int16_t> samples;
    //double value;
    //while (in >> value) {
    //    if (in.peek() == ',' || in.peek() == '\n') in.ignore();
    //    // Zak³adamy amplitudy w zakresie -1.0 do 1.0
    //    int16_t sample = static_cast<int16_t>(value * 32767.0);
    //    samples.push_back(sample);
    //}

    //in.close();

    //WAVHeader header;
    ////header.sampleRate = static_cast<uint32_t>(czestotliwoscProbkowania);
    //header.dataSize = samples.size() * sizeof(int16_t);
    //header.chunkSize = 36 + header.dataSize;
    //header.byteRate = header.sampleRate * header.numChannels * header.bitsPerSample / 8;
    //header.blockAlign = header.numChannels * header.bitsPerSample / 8;

    //std::ofstream out(wavFile, std::ios::binary);
    //if (!out.is_open()) {
    //    std::cerr << "Nie mozna utworzyc pliku WAV\n";
    //    return;
    //}

    //out.write(reinterpret_cast<const char*>(&header), sizeof(WAVHeader));
    //out.write(reinterpret_cast<const char*>(samples.data()), header.dataSize);
    //out.close();

    //std::cout << "Plik WAV zapisany jako: " << wavFile << "\n";
    return;
    //Sleep(100);
}

enum Voices Fala::grajDzwiek()
{
	return Voices::OSCYLATOR_A;
}
////////////
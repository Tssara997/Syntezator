#include "Fala.h"

void Fala::draw() const
{
    std::ofstream file("dane.csv");

    // try
   /* if (!file.is_open()) {
        std::cerr << "Nie otworzono pliku\n";
        return 1;
    }*/

    for (double x = 0; x <=czestotliwosc; x += 1) {
        double y =  amplituda * sin(2 * M_PI * czestotliwosc * x/czestotliwoscProbkowania  + przesuniecieFazy);
        file << y << "\n";
    }

    file.close();
}

void Fala::zmienFaze(double x)
{
    przesuniecieFazy = x;
}

double Fala::getFaza() const
{
    return przesuniecieFazy;
}

void Fala::grajDzwiek()
{
    //draw();
    std::string csvFile = "dane.csv";
    std::string wavFile = "output.wav";
    std::ifstream in(csvFile);
    if (!in.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku csv\n";
        return;
    }

    std::vector<int16_t> samples;
    double value;
    while (in >> value) {
        if (in.peek() == ',' || in.peek() == '\n') in.ignore();
        // Zak³adamy amplitudy w zakresie -1.0 do 1.0
        int16_t sample = static_cast<int16_t>(value * 32767.0);
        samples.push_back(sample);
    }

    in.close();

    WAVHeader header;
    //header.sampleRate = static_cast<uint32_t>(czestotliwoscProbkowania);
    header.dataSize = samples.size() * sizeof(int16_t);
    header.chunkSize = 36 + header.dataSize;
    header.byteRate = header.sampleRate * header.numChannels * header.bitsPerSample / 8;
    header.blockAlign = header.numChannels * header.bitsPerSample / 8;

    std::ofstream out(wavFile, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Nie mozna utworzyc pliku WAV\n";
        return;
    }

    out.write(reinterpret_cast<const char*>(&header), sizeof(WAVHeader));
    out.write(reinterpret_cast<const char*>(samples.data()), header.dataSize);
    out.close();

    std::cout << "Plik WAV zapisany jako: " << wavFile << "\n";

    Sleep(100);

    PlaySound(
        TEXT("output.wav"),
        NULL,
        SND_FILENAME | SND_ASYNC | SND_LOOP
    );
}

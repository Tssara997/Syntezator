#include "Fala.h"

const std::string Fala::draw() const
{
    std::ostringstream ss;
    for (double x = 0; x <= 2 * M_PI; x += 0.1) {
        double y = amplituda * sin(czestotliwosc * x  + przesuniecieFazy);
        ss << x << "," << y << "\n";
    }
    return ss.str();
}

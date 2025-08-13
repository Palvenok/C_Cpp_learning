#ifndef VECTOR2INT
#define VECTOR2INT

#include <cmath>
#include <string>

class vector2int
{
private:
    int x;
    int y;

public:
    const int& X = x;
    const int& Y = y;

    vector2int();
    vector2int(int, int);

    int magnitude();
    std::string to_string();

    static vector2int distance(vector2int& vec1, vector2int& vec2);
};

#endif
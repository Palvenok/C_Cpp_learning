#ifndef VECTOR2INT
#define VECTOR2INT

#include <cmath>
#include <string>

struct vector2int
{
    int x;
    int y;

    vector2int();
    vector2int(int, int);

    int magnitude();
    std::string to_string();

    static vector2int distance(vector2int& vec1, vector2int& vec2);
};

#endif
#include "vector2int.h"

vector2int::vector2int()
{
    x = 0;
    y = 0;
}

vector2int::vector2int(int x, int y)
{
    this->x = x;
    this->y = y;
}

int vector2int::magnitude()
{
    return sqrt(x*x + y*y);
}

std::string vector2int::to_string()
{
    return "x: " + std::to_string(x) + "  y: " + std::to_string(y);
}

vector2int vector2int::distance(vector2int& vec1, vector2int& vec2)
{
    return vector2int(vec1.x - vec2.x, vec1.y - vec2.y);
}
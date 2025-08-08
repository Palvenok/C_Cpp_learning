#include <iostream>
#include <string>
#include "vector2int.h"

int main()
{
    vector2int vec1 = vector2int(13, 39);
    std::cout << vec1.to_string() 
                << "   magnitude: " << vec1.magnitude() 
                << std::endl;
                
    vector2int vec2 = vector2int(80, 13);
    std::cout << vec2.to_string() 
                << "   magnitude: " << vec2.magnitude() 
                << std::endl;

    vector2int vec3 = vector2int::distance(vec1, vec2);
    std::cout << vec3.to_string() 
                << "   magnitude: " << vec3.magnitude() 
                << std::endl;
    return 0;
}
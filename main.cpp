#include <iostream>
#include <vector>

#include "shape.h"

using namespace std;

void drawShapes(const std::vector<Shape>& shapes)
{
    for (const auto& shape : shapes)
    {
        if (const auto* circle = cast<Circle>(&shape))
        {
            std::cout << "a circle\n";
        }
        else if (const auto* square = cast<Square>(&shape))
        {
            std::cout << "a square\n";
        }
    }
}

int main()
{
    Circle circle(3.0);
    Square square(5.0);

    std::vector<Shape> v;
    v.emplace_back(std::move(circle));
    v.emplace_back(std::move(square));

    drawShapes(v);

    return 0;
}

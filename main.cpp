#include <iostream>
#include <vector>

#include "shape.h"

using namespace std;

void drawShapes(const std::vector<Shape>& shapes)
{
    for (const auto& shape : shapes)
    {
        if (const Circle* circle = cast<const Circle*>(&shape))
        {
            std::cout << "a circle\n";
        }
    }
}

int main()
{
    Circle circle(3.0);
    Circle& circleRef(circle);

    const Circle constCircle(3.0);
    const Circle& constCircleRef(constCircle);

    std::vector<Shape> v;
    v.emplace_back(std::move(circle));
    v.emplace_back(std::move(circleRef));

    v.emplace_back(constCircle);
    v.emplace_back(constCircleRef);

    v.emplace_back(Circle(3.0));

    drawShapes(v);

    return 0;
}

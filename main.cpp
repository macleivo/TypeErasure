#include <iostream>
#include <variant>
#include <vector>

#include "shape.h"

void drawShapes(std::vector<Shape>& shapes)
{
    for (const auto& shape : shapes)
    {
        if (const auto* circle = cast<const Circle*>(&shape))
        {
            std::cout << "castable to a const circle\n";
        }
    }

    for (auto& shape : shapes)
    {
        if (auto* circle = cast<Circle*>(&shape))
        {
            std::cout << "castable to a non-const circle\n";
        }
    }
}

std::vector<Shape> undefined_behavior()
{
    std::vector<Shape> v;
    {
        Circle circle(3.0);
        Circle& circleRef(circle);

        const Circle constCircle(3.0);
        Circle const& constCircleRef(constCircle);

        v.emplace_back(circle);
        v.emplace_back(circleRef);
        v.emplace_back(constCircle);
        v.emplace_back(constCircleRef);
    }
    return v;
}

int main()
{

#if 0
    auto v = undefined_behavior();
#else
    std::vector<Shape> v;
    Circle circle(3.0);
    Circle& circleRef(circle);

    const Circle constCircle(3.0);
    Circle const& constCircleRef(constCircle);

    v.emplace_back(circle);
    v.emplace_back(circleRef);
    v.emplace_back(constCircle);
    v.emplace_back(constCircleRef);
#endif

    drawShapes(v);

    return 0;
}

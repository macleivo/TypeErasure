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
        draw(shape);
    }

    for (auto& shape : shapes)
    {
        if (auto* circle = cast<Circle*>(&shape))
        {
            std::cout << "castable to a non-const circle\n";
        }
    }
}

std::vector<Shape> foo()
{

    std::vector<Shape> v;
    {
        Circle circle(3.0);
        Circle& circleRef(circle);

        const Circle constCircle(3.0);
        Circle const& constCircleRef(constCircle);

        std::cout << "circle: " << &circle << std::endl;
        std::cout << "circle ref: " << &circleRef << std::endl;
        std::cout << "const circle: " << &constCircle << std::endl;
        std::cout << "const circle ref: " << &constCircleRef << std::endl;

        v.emplace_back(circle);
        v.emplace_back(circleRef);
        v.emplace_back(constCircle);
        v.emplace_back(constCircleRef);
    }
    return v;
}

int main()
{

    auto v = foo();

    drawShapes(v);

    return 0;
}

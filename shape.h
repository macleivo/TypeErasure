#pragma once

#include <iostream>
#include <memory>
#include <type_traits>

class Circle;
class Square;

class Shape
{
public:
    template<typename T, typename = std::enable_if_t<not std::is_same_v<std::remove_cvref_t<T>, Shape>>>
    explicit Shape(T&& t) : m_impl(std::make_unique<Model<T>>(std::forward<T>(t)))
    {
    }

    friend void drawShape(const Shape& shape)
    {
        shape.m_impl->doDraw();
    }

    template<typename T>
    friend const T* cast(const Shape* shape)
    {
        if (auto* model = dynamic_cast<Model<T>*>(shape->m_impl.get()))
        {
            return &model->m_t;
        }
        return nullptr;
    }

    template<typename T>
    friend T* cast(Shape* shape)
    {
        const Shape* tmp = shape;
        return const_cast<T*>(cast<T>(tmp));
    }

private:
    struct Concept
    {
        virtual ~Concept() = default;
        virtual void doDraw() const = 0;
    };

    template<typename T>
    struct Model : Concept
    {
        Model(T&& t) : m_t(std::forward<T>(t))
        {
        }

        void doDraw() const final
        {
            draw(m_t);
        }

        T m_t;
    };

    std::unique_ptr<Concept> m_impl;
};

class Circle
{
public:
    explicit Circle(double radius) : m_radius(radius)
    {
    }

    double radius() const
    {
        return m_radius;
    }

private:
    double m_radius;
};

void draw(const Circle& circle)
{
    std::cout << "Circle with radius " << circle.radius() << std::endl;
}

class Square
{
public:
    explicit Square(double side) : m_side(side)
    {
    }

    double side() const
    {
        return m_side;
    }

private:
    double m_side;
};

void draw(const Square& square)
{
    std::cout << "Square with side " << square.side() << std::endl;
}

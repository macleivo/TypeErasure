#pragma once

#include <iostream>
#include <memory>
#include <type_traits>

class Circle;
class Square;
class Shape;

template<typename T>
concept AcceptedTypeForShape = not std::is_same_v<std::remove_cvref_t<T>, Shape> && not std::is_pointer_v<T>;

class Shape
{
public:
    template<AcceptedTypeForShape T>
    explicit Shape(T&& t) : m_impl(std::make_unique<Model<T>>(std::forward<T>(t)))
    {
    }

    friend void draw(const Shape& shape)
    {
        shape.m_impl->doDraw();
    }

    template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
    friend T cast(const Shape* shape)
    {
        using cleanT = std::remove_cvref_t<std::remove_pointer_t<T>>;
        if (auto* model = dynamic_cast<Model<cleanT>*>(shape->m_impl.get()))
        {
            return &model->m_t;
        }
        if (auto* model = dynamic_cast<Model<cleanT&>*>(shape->m_impl.get()))
        {
            return &model->m_t;
        }
        if (auto* model = dynamic_cast<Model<const cleanT&>*>(shape->m_impl.get()))
        {
            return &model->m_t;
        }
        //                static_assert(std::is_same_v<T, cleanT> && false);
        return nullptr;
    }

    template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
    friend T cast(Shape* shape)
    {
        const auto* tmp = shape;
        return cast<T>(tmp);
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

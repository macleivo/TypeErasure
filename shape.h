#pragma once

#include <iostream>
#include <memory>
#include <type_traits>

class Circle;
class Square;
class Shape;

template<typename T>
concept AcceptedTypeForShape = not std::is_same_v<std::remove_cvref_t<T>, Shape> && not std::is_pointer_v<T>;

template<typename To, typename From>
To cast(const From* from)
{
    return from->template cast<To>();
}

template<typename T>
class Castable
{
    template<typename To, typename From>
    friend To cast(const From* from);

private:
    template<typename To>
    To cast() const
    {
        auto* derived = static_cast<const T*>(this);
        using cleanT = std::remove_cvref_t<std::remove_pointer_t<To>>;
        if (auto* model = dynamic_cast<typename T::template Model<cleanT>*>(derived->m_impl.get()))
        {
            return &model->m_t;
        }
        if (auto* model = dynamic_cast<typename T::template Model<cleanT&>*>(derived->m_impl.get()))
        {
            return &model->m_t;
        }
        if (auto* model = dynamic_cast<typename T::template Model<const cleanT&>*>(derived->m_impl.get()))
        {
            return &model->m_t;
        }
        //                static_assert(std::is_same_v<T, cleanT> && false);
        return nullptr;
    }
};

class Shape : public Castable<Shape>
{
    friend class Castable<Shape>;

public:
    template<AcceptedTypeForShape T>
    explicit Shape(T&& t) : m_impl(std::make_unique<Model<T>>(std::forward<T>(t)))
    {
    }

    friend void draw(const Shape& shape)
    {
        shape.m_impl->doDraw();
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

#pragma once

#include <iostream>
#include <memory>
#include <type_traits>

class Circle;
class Square;
class Shape;

template<typename T>
struct remove_cvref
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

template<typename To, typename From>
To cast(const From* from)
{
    using cleanT = remove_cvref_t<std::remove_pointer_t<To>>;

    const bool isConst = from->m_impl->m_isConst;
    const bool isReference = from->m_impl->m_isReference;

    if (isConst && isReference)
    {
        if (const auto* model = dynamic_cast<const typename From::template Model<const cleanT&>*>(from->m_impl.get()))
        {
            return &model->m_t;
        }
        return nullptr;
    }

    if (isReference)
    {
        if (const auto* model = dynamic_cast<const typename From::template Model<cleanT&>*>(from->m_impl.get()))
        {
            return &model->m_t;
        }
        return nullptr;
    }

    if (const auto* model = dynamic_cast<const typename From::template Model<cleanT>*>(from->m_impl.get()))
    {
        return &model->m_t;
    }

    return nullptr;
}

template<typename To, typename From>
To cast(From* from)
{
    using cleanT = remove_cvref_t<std::remove_pointer_t<To>>;
    const bool isConst = from->m_impl->m_isConst;
    if (isConst)
    {
        return nullptr;
    }

    const bool isReference = from->m_impl->m_isReference;
    if (isReference)
    {
        if (auto* model = dynamic_cast<typename From::template Model<cleanT&>*>(from->m_impl.get()))
        {
            return &model->m_t;
        }
        return nullptr;
    }

    if (auto* model = dynamic_cast<typename From::template Model<cleanT>*>(from->m_impl.get()))
    {
        return &model->m_t;
    }
    return nullptr;
}

template<typename T>
struct TD;

class Shape
{
    template<typename To, typename From>
    friend To cast(const From* from);

    template<typename To, typename From>
    friend To cast(From* from);

public:
    template<typename T>
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

        Concept(bool isConst, bool isReference, bool isPointer)
            : m_isConst(isConst), m_isReference(isReference), m_isPointer(isPointer)
        {
        }

        // used in casting
        const bool m_isConst {};
        const bool m_isReference {};
        const bool m_isPointer {};
    };

    template<typename T>
    struct Model : Concept
    {
        Model(T&& t)
            : Concept(std::is_const_v<std::remove_reference_t<T>>, std::is_reference_v<T>, std::is_pointer_v<T>),
              m_t(std::forward<T>(t))
        {
            std::cout << &m_t << std::endl;
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
    Circle(const Circle& circ) : m_radius(circ.m_radius)
    {
        std::cout << "copy ctor of circle\n";
    }

    Circle& operator=(const Circle& circ)
    {
        m_radius = circ.m_radius;
        std::cout << "copy assign of circle\n";
        return *this;
    }

    Circle(Circle&& circ) : m_radius(circ.m_radius)
    {
        std::cout << "move ctor of circle\n";
    }

    Circle& operator=(Circle&& circ)
    {
        m_radius = circ.m_radius;
        std::cout << "move assign of circle\n";
        return *this;
    }

    ~Circle() = default;

    explicit Circle(double radius) : m_radius(radius)
    {
        std::cout << "ctor of circ\n";
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

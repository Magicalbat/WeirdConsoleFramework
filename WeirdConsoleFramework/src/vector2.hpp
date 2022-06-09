#pragma once

#include <cmath>

namespace wcf
{
    struct Vector2
    {
        float x;
        float y;

        Vector2()
            :x(0), y(0)
        {}

        Vector2(float x, float y)
            :x(x), y(y)
        {}

        Vector2(const Vector2& other)
            :x(other.x), y(other.y)
        {}

        float length() const { return sqrt(x * x + y * y); }
        float lengthSquared() const { return x * x + y * y; }

        Vector2 normalized() const
        {
            float l = length();
            float r = 1.0f;
            if (l != 0.0f)
                r = 1.0f / l;
            return { x * r, y * r };
        }

        Vector2 perpendicular() const { return { -y, x }; }

        float dot(const Vector2& other) const { return x * other.x + y * other.y; }
        float cross(const Vector2& other) const { return x * other.y - y * other.x; }

        float distance(const Vector2& other) const
        {
            return sqrt(((other.x - x) * (other.x - x)) + ((other.y - y) * (other.y - 1)));
        }
        float distanceSquared(const Vector2& other) const
        {
            return ((other.x - x) * (other.x - x)) + ((other.y - y) * (other.y - 1));
        }

        Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
        Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
        Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
        Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

        Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
        Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
        Vector2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
        Vector2& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

        bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
        bool operator!=(const Vector2& other) const { return x != other.x || y != other.y; }
    };
}
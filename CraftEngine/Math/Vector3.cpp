#include"Vector3.h"
#include "MathDefines.h"
#include <cmath>
#include <cassert>
namespace Craft
{
    const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
    const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);
    const Vector3 Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);

    Vector3::Vector3(float x, float y, float z)
        : x(x), y(y), z(z)
    {
    }

    Vector3::Vector3(const Vector3& other)
        : x(other.x), y(other.y), z(other.z)
    {
    }

    float Vector3::Length() const
    {

        return std::sqrt(x * x + y * y + z * z);
    }

    float Vector3::LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    Vector3 Vector3::Normalized() const
    {
        float length = Length();
        if (length <= KindaSmallNumber)
        {
            assert(false && "length should not be zero");
            return Vector3::Zero;
        }
        return Vector3(x / length, y / length, z / length);
    }

    float Vector3::Dot(const Vector3& other) const
    {

        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 Vector3::Cross(const Vector3 other) const
    {
        //
        return Vector3(
            y * z - other.y * z,
            z * other.x-  x * other.z,
            x * other.y - y * other.x
        );

        return Vector3();
    }

    bool Vector3::operator==(const Vector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool Vector3::operator!=(const Vector3& other) const
    {
        return !(*this == other);
    }

    Vector3& Vector3::operator=(const Vector3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vector3 Vector3::operator+(const Vector3& other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3& Vector3::operator+=(const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3 Vector3::operator-(const Vector3& other) const
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3& Vector3::operator-=(const Vector3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3 Vector3::operator*(const Vector3& other) const
    {
        return Vector3(x * other.x, y * other.y, z * other.z);
    }

    Vector3 Vector3::operator*(float scale) const
    {
        return Vector3(x * scale, y * scale, z * scale);
    }

    Vector3& Vector3::operator*=(const Vector3& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vector3& Vector3::operator*=(float scale)
    {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }

    Vector3 Vector3::operator/(const Vector3& other) const
    {
        if (std::abs(other.x) <= KindaSmallNumber
            || std::abs(other.y) <= KindaSmallNumber
            || std::abs(other.z) <= KindaSmallNumber)
        {
            assert(false && "other.x, other.y and other.z should not be near 0");
            return Vector3::Zero;
        }
        return Vector3(x / other.x, y / other.y, z / other.z);
    }

    Vector3 Vector3::operator/(float scale) const
    {
        if (std::abs(scale) <= KindaSmallNumber)
        {
            assert(false && "scale should not be near 0");
            return Vector3::Zero;
        }
        return Vector3(x / scale, y / scale, z / scale);
    }

    Vector3& Vector3::operator/=(const Vector3& other)
    {
        if (std::abs(other.x) <= KindaSmallNumber
            || std::abs(other.y) <= KindaSmallNumber
            || std::abs(other.z) <= KindaSmallNumber)
        {
            assert(false && "other.x, other.y and other.z should not be near 0");
            return *this;
        }
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    Vector3& Vector3::operator/=(float scale)
    {
        if (std::abs(scale) <= KindaSmallNumber)
        {
            assert(false && "scale should not be near 0");
            return *this;
        }
        x /= scale;
        y /= scale;
        z /= scale;
        return *this;
    }

}
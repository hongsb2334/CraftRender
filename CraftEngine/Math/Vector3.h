#pragma once

namespace Craft
{
    //행벡터로 사용될 3차원 벡터
    class Vector3
    {
    public:
        Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
        Vector3(const Vector3& other);
        ~Vector3() = default;

        //길이 및 내적
        float Length() const;
        float LengthSquared() const;
        Vector3 Normalized() const;
        float Dot(const Vector3& other) const;

        //외적(Cross Product)
        Vector3 Cross(const Vector3 other) const;



        bool operator==(const Vector3& other) const;
        bool operator!=(const Vector3& other) const;
        Vector3& operator=(const Vector3& other);

        Vector3 operator+(const Vector3& other) const;
        Vector3& operator+=(const Vector3& other);
        Vector3 operator-(const Vector3& other) const;
        Vector3& operator-=(const Vector3& other);

    public:
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Right;
        static const Vector3 Up;
        static const Vector3 Forward;
    };
}
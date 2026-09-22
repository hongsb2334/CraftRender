#include "Vector2.h"
#include "MathDefines.h"
#include <cmath>
#include <cassert>

namespace Craft
{
    //static 변수 설정
    const Vector2 Vector2::Zero(0.0f, 0.0f);
    const Vector2 Vector2::One(1.0f, 1.0f);
    const Vector2 Vector2::Right(1.0f, 0.0f);
    const Vector2 Vector2::Up(1.0f, 1.0f);

    Vector2::Vector2(float x, float y) : x(x), y(y)
    {
    }
    float Vector2::Length() const
    {
        //피타고라스 정리 활용

        return std::sqrt(x * x + y * y);
    }
    float Vector2::LengthSquared() const
    {
        return x * x + y * y;
    }
    Vector2 Vector2::Normalized() const
    {
        float length = Length();
        //벡터 성분에서 길이 나눠서 정규화
        if (length <= KindaSmallNumber)
        {
            assert(false && "length should not be zero");
            return Vector2::Zero;
        }
        return Vector2(x/length, y/length);
    }
    float Vector2::Dot(const Vector2& other)
    {
        //두 가지 방식으로 계산 가능
        //엔진에서는 쉬운 계산 법으로 구현
        // a dot b = |A| * |B| * cos(theta)
        return x * other.x + y * other.y;
    }
    bool Vector2::operator==(const Vector2& other) const
    {

        return x == other.x && y == other.y;
    }
    bool Vector2::operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }
    Vector2& Vector2::operator=(const Vector2& other)
    {
        //memcpy(this, &other, sizeof(Vector2));

        x = other.x;
        y = other.y;
        return *this;
        // TODO: insert return statement here
    }
}
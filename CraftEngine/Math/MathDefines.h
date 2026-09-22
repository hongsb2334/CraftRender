#pragma once


namespace Craft
{
    //PI, 컴파일 타임 상수
    constexpr float Pi = 3.141592f;

    //0에 가까운 수 정의
    constexpr float KindaSmallNumber = 1e-4f;

    //라디안 <-> 각도 변환 값
    //2pi 라디안 = 360도 1pi = 180도
    constexpr float DegreesToRadians = Pi / 180.0f;

    constexpr float RadiansToDegrees = 180.0f / Pi;

}
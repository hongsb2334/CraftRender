#pragma once
#include <Math/Vector2.h>
#include <Math/Vector3.h>
namespace Craft
{
    //메시의 기본 정보인 정점 구조체
    struct Vertex
    {
        //위치
        Vector3 position;

        //텍스처 좌표(UV)
        Vector2 uv;

    };

}
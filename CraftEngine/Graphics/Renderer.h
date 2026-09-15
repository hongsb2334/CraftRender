#pragma once

//direct3d 라이브러리의 객체를 사용하기 위한 인클루드

#include <d3d11.h>
#include <dxgi.h>

namespace Craft
{
    //cpu에서 gpu로 명령을 전달
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();
    
    private:
        //장치 생성
        void CreateDevices();
        //스왑 체인 생성


    private:
        //장치들(그래픽카드)

        //디바이스->데이터 생성
        ID3D11Device* device = nullptr;
        
        //그래픽카드에 데이터 설정(연결/바인딩)
        ID3D11DeviceContext* context = nullptr;

        //기능 변경이 거의 없는 장치
        IDXGISwapChain* swapChain = nullptr;

    };
}



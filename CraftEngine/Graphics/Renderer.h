#pragma once

#include<Core/Core.h>

//direct3d 라이브러리의 객체를 사용하기 위한 인클루드
#include <d3d11.h>
#include <dxgi.h>
#include <cstdint>

namespace Craft
{
    //전방선언
    class Win32Window;

    //cpu에서 gpu로 명령을 전달
    class Renderer
    {
    public:
        Renderer(const Win32Window& window);
        ~Renderer();

        //Draw 함수
        void Draw(float red, float green, float blue, uint32_t vsync);
    
    private:
        //그리기 단계 별로 실행되는 함수
        
        //그리기 준비 단계
        void BeginScene(float red, float green, float blue);
        //장면 그리기
        void DrawScene();
        //버퍼 교환
        void EndScene(uint32_t vsync);
        
        //장치 생성
        void CreateDevices();
        
        //스왑 체인 생성
        void CreateSwapChain(const Win32Window& window);

        //렌더 타겟 뷰 생성
        void CreateRenderTargetView();

        //데모를 위한 버퍼 생성 함수
        //버퍼: 메모리 덩어리
        void CreateDemoBuffers();

        //기본 셰이더 생성 함수
        void CreateDefaultShaders();


    private:
        //장치들(그래픽카드)

        //디바이스->데이터 생성
        ID3D11Device* device = nullptr;
        
        //그래픽카드에 데이터 설정(연결/바인딩)
        ID3D11DeviceContext* context = nullptr;

        //기능 변경이 거의 없는 장치
        IDXGISwapChain* swapChain = nullptr;

        //백버퍼를 대표하는 렌더 타겟
        //그래픽카드에 그릴 대상을 선정할 때 사용하는 타입
        ID3D11RenderTargetView* renderTargetView = nullptr;
        
        //정점 버퍼(VertexBuffer)
        ID3D11Buffer* vertexBuffer = nullptr;

        //인덱스 버퍼(색인 버퍼)
        ID3D11Buffer* indexBuffer = nullptr;
 
        //셰이더 관련 변수
        ID3D11VertexShader* vertexShader = nullptr;
        ID3D11PixelShader* pixelShader = nullptr;

        //입력의 정보를 전달하는 데 사용되는 객체
        ID3D11InputLayout* inputLayout = nullptr;

     

    };
}



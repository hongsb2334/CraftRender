#pragma once
#include <d3d11.h>
#include <string>
#include <memory>


namespace Craft
{
    //d3d11 셰이더 객체를 관리하는 클래스
    class Shader
    {
    public:
        //vertex/pixel shader 이름 받는거에서 shadername 하나 받는 것으로 변경
        Shader(const std::wstring& shaderName);
        ~Shader();

        //그래픽스 파이프라인에 셰이더 객체 바인딩
        void Bind();

        //편의 함수 - 기본 셰이더 생성 함수
        static std::shared_ptr<Shader> CreateDefaultShader();

    private:
        //정점 셰이더 객체
        ID3D11VertexShader* vertexShader = nullptr;

        //픽셀 셰이더 객체
        ID3D11PixelShader* pixelShader = nullptr;

        //입력 레이아웃(정점 셰이더 입력의 명세서)
        ID3D11InputLayout* inputLayout = nullptr;
    };
}



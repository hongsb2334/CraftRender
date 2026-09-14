#pragma once
#include <memory>
#include <string>

namespace Craft
{

    //전방선언
    class Win32Window;
    class Engine
    {
    public:
        Engine(
            uint32_t width = 1280,
            uint32_t height = 800,
            const std::wstring title = L"Craft Render Engine");
        
        virtual ~Engine();

        //엔진 루프 실행 함수
        void Run();

        //엔진 종료 함수
        void Quit();

    protected:
        //창 객체
        std::unique_ptr<Win32Window> window;
    };
}
#pragma once
#include <Interface/IMessageHandler.h>
#include <GameFramework/Level.h>
#include <memory>
#include <string>
#include <cstdint>

namespace Craft
{

    //전방선언
    class Win32Window;
    class Renderer;


    class Engine : public IMessageHandler
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

        //전역 접근 함수
        static Engine& Get();


        //레벨 추가 함수
        template<typename T, typename = std::enable_if<std::is_base_of<Level, T>::Value>>
        std::shared_ptr<T> AddNewLevel()
        {
            //새 레벨 생성 후 nextLevel로 지정
            std::shared_ptr<T> newLevel = std::shared_ptr<T>();
            nextLevel = newLevel;


        }

    protected:
        
        //BeginPlay/Tick 함수
        
        void BeginPlay();
        void Tick(float deltaTime);
        
        //Draw 함수
        void Draw();

        //창 크기 변경 이벤트 함수
        void OnResize(uint32_t width, uint32_t height);

    protected:
        // IMessageHandler을(를) 통해 상속됨
        virtual LRESULT HandleMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam) override;

    protected:

        //엔진 전역 접근
        inline static Engine* instance = nullptr;


        //엔진 종료 플래그
        bool isQuit = false;

        //창 객체
        std::unique_ptr<Win32Window> window;

        //렌더러 객체
        std::unique_ptr<Renderer> renderer;

        //메인 레벨
        std::shared_ptr<Level> MainLevel;

        //추가 요청된 레벨
        std::shared_ptr<Level> NextLevel;

    };
}
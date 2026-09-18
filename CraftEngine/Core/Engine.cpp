#include "Engine.h"
#include "Core/Win32Window.h"
#include <Graphics/Renderer.h>

#if _DEBUG
#include <iostream>
#endif


namespace Craft
{
    Engine::Engine(uint32_t width, uint32_t height, const std::wstring title)
    {
        //창 객체 생성
        //this넘기는건 부모인 인터페이스로 업캐스팅해서 넘김
        window = std::make_unique<Win32Window>(width, height, this, title);

        //렌더러 객체 생성
        renderer = std::make_unique<Renderer>(*window);
    }

    Engine::~Engine()
    {
    }

    void Engine::Run()
    {
        //고해상도 타이머 주파수 가져오기
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);

        //deltaTime 구하기를 람다로
        auto GetDeltaTime = [&frequency](int64_t& current, int64_t& previous)
            {
                LARGE_INTEGER counter;
                QueryPerformanceCounter(&counter);
                current = counter.QuadPart;

                return static_cast<float>(current - previous) / static_cast<float>(frequency.QuadPart);
            };

        //프레임 계산을 위한 변수
        int64_t current = 0;
        int64_t previous = 0;

        //고정 프레임 처리위한 값
        const float framerate = 120.0f;
        const float oneFrameTime = 1.0f / framerate;
        
        //스레드 재우는 간격 설정
        timeBeginPeriod(1);

        //이벤트(창 메시지 처리 루프)
        MSG message = {};
        while (message.message != WM_QUIT)
        {
            //min/max설정 해도되고 0 넣어도 다 받아줌
            //창에 메시지가 발생한 경우의 처리
            if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            //엔진 루프 처리 등
            else
            {
                //프레임 시간 구하기
                float deltaTime = GetDeltaTime(current, previous);

                //대기 시간 계산
                float remainingTime = oneFrameTime - deltaTime;

                //스레드 재우기
                while (remainingTime >= 0.002f)
                {
                    //Todo: 멀티스레드 환경에서 Sleep에서 0넣을때와 1넣을때 차이점 찾기
                    Sleep(1);

                    //프레임 시간 구하기
                    deltaTime = GetDeltaTime(current, previous);
                    
                    //남은 시간 계산
                    remainingTime = oneFrameTime - deltaTime;
                }

                while (remainingTime > 0.0f)
                {
                    //프레임 시간 구하기
                    deltaTime = GetDeltaTime(current, previous);

                    //남은 시간 계산
                    remainingTime = oneFrameTime - deltaTime;
                }

#if _DEBUG
                std::cout << "deltaTime : " << deltaTime << " | FPS : " << (1.0f / deltaTime) << "\n";
#endif

                Draw();


                //이전 시간 기록
                previous = current;
            }
        }

        timeEndPeriod(1);
        
    }

    void Engine::Quit()
    {
    }

    void Engine::Draw()
    {
        //이벤트 전달
        if (renderer)
        {
            //0으로 설정해서 vsync 안쓰게
            renderer->Draw(0.6f, 0.7f, 0.8f, 0);
        }
    }

    LRESULT Engine::HandleMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
    {
        //엔진이 윈도우를 소유
        //윈도우가 엔진에 접근하려면 인터페이스 사용, 상호 참조하는것은 좋지 않음
        //메시지 처리
        switch (message)
        {
            //창 닫기 메시지 (x 버튼 누르면 발생)
        case WM_CLOSE:
        {
            //창 객체 삭제
            DestroyWindow(window);
        }
        return 0;
        //창 삭제 이벤트 처리
        case WM_DESTROY:
        {
            // 프로그램 종료 요청.(종료 메시지 발행) return 0; 에서의 0
            PostQuitMessage(0);
        }
        return 0;

        //ESC키 입력 처리
        case WM_KEYDOWN:
        {
            //눌린 키가 ESC키인지 확인
            if (wparam == VK_ESCAPE)
            {
                DestroyWindow(window);
            }
        }
        return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(window, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            EndPaint(window, &ps);
        }
        return 0;
        }
        return DefWindowProc(window, message, wparam, lparam);
    }
}


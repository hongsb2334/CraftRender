#include "LaunchApplication.h"
#include <cstdint>
//함수 선언
LRESULT Win32MessageHandler(HWND window, UINT message , WPARAM wparam, LPARAM lparam)
{
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
    }

    return DefWindowProc(window, message, wparam, lparam);
}

int LaunchApplication(HINSTANCE instance)
{
    //창 만들기
    // Register the window class.
    const wchar_t* className = L"Craft_Render_Engine_Class";

    WNDCLASS wc = { };

    //창 만들때 사용되는 정보 구조체
    wc.lpfnWndProc = Win32MessageHandler;
    wc.hInstance = instance;
    wc.lpszClassName = className;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    //클래스 등록 전달되는 클래스 이름은 고유값이어야 함
    if (!RegisterClass(&wc))
    {
        return -1;
    }

    //프레임 크기 기본 설정
    uint32_t width = 1280;
    uint32_t height = 800;

    //창 크기 조정
    RECT rect = {0, 0, (long)width, (long)height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    //창 크기 계산
    uint32_t windowWidth = rect.right - rect.left;
    uint32_t windowHeight = rect.bottom - rect.top;

    //화면 가운데 창 생성하도록 위치 구하기
    uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
    uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;

    // Create the window.

    HWND hwnd = CreateWindow(
        className,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // position and Size  
        x, y, windowWidth, windowHeight,

        nullptr,       // Parent window    
        nullptr,       // Menu
        instance,  // Instance handle
        nullptr        // Additional application data
    );

    if (hwnd == nullptr)
    {
        return 0;
    }

    GetClientRect(hwnd, &rect);


    //창 보이기 모드 설정
    ShowWindow(hwnd, SW_SHOW);

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

        }
    }
    return 0;
}

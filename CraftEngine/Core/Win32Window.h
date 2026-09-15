#pragma once

#include <Windows.h>
#include <string>
#include <cstdint>

namespace Craft
{
    //전방 선언
    class IMessageHandler;

	class Win32Window
	{
	public:
        Win32Window(
            uint32_t width = 1280,
            uint32_t height = 800,
            IMessageHandler* messageHandler = nullptr,
            const std::wstring title = L"Craft Render Engine");

        ~Win32Window();

    private: 
        //창 메시지 처리 함수
        //static으로 하면 함수포인터에 전달 가능하다.
        static LRESULT Win32MessageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

	private:
		//프로그램 인스턴스(포인터)
        HINSTANCE instance = nullptr;

        //창 객체 핸들 (포인터)
        HWND handle = nullptr;
        
        //창 크기
        uint32_t width = 0;
        uint32_t height = 0;

        //클래스 이름/ 타이틀 이름
        std::wstring className = L"Craft_Render_Window_Class";
        std::wstring title;

        //메시지 핸들러 객체
        IMessageHandler* messageHandler = nullptr;

	};
}



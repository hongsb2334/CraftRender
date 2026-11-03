#pragma once
#pragma warning(disable: 4251)

#include <Windows.h>
#include <comdef.h>
#include <string>
#include <cassert>


#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#if BUILD_ENGINE_DLL
#define CRAFT_API DLLEXPORT
#else
#define CRAFT_API DLLIMPORT
#endif

#define TO_WIDE(x) L##x

#define FILENAME_TO_WIDE(x) TO_WIDE(x)
#define GET_FILE_NAME FILENAME_TO_WIDE(__FILE__)

#define FUNCTION_TO_WIDE(x) TO_WIDE(x)
#define GET_FUNCTION_NAME FUNCTION_TO_WIDE(__FUNCTION__)

//Todo: buffer 동적할당으로 바꾸기

#define ThrowIfFailed(result, message)														\
	do																						\
	{																						\
		if (FAILED(result))																	\
		{																					\
			_com_error error(result);														\
			const wchar_t* errorMessage = error.ErrorMessage();								\
			std::wstring buffer = L"File: " + std::wstring(GET_FILE_NAME) +					\
								  L"\nFunction: " + std::wstring(GET_FUNCTION_NAME) +		\
								  L"\nLine: " + std::to_wstring(__LINE__) +					\
								  L"\nMessage: " + std::wstring(message) +					\
								  L"\nError:" + std::wstring(errorMessage) + L"\n";			\
			MessageBox(nullptr, buffer.c_str(), L"D3D Error", MB_OK);						\
			__debugbreak();																	\
		}																					\
	} while (false)


template<typename T>
void SafeRelease(T*& t)
{
	if (t)
	{
		t->Release();
		t = nullptr;
	}
}

//전역 접근 가능한 클래스 기능 제공하는 클래스 템플릿
template<typename T>
class TGlobalAccessible
{
public:

    TGlobalAccessible()
    {
        assert(!instance);
        instance = static_cast<T*>(this);
    }

    virtual ~TGlobalAccessible()
    {
        instance = nullptr;
    }

    //복사/이동 방지
    TGlobalAccessible(const TGlobalAccessible& other) = delete;

    TGlobalAccessible& operator=(TGlobalAccessible& other) = delete;

    TGlobalAccessible(const TGlobalAccessible&& other) = delete;

    TGlobalAccessible& operator=(TGlobalAccessible&& other) = delete;

    //전역 접근 변수
    static T& Get()
    {
        assert(instance);
        return *instance;
    }

    //객체 접근이 유효한지 확인
    static bool IsValid()
    {
        return instance != nullptr;
    }


private:
    //전역 변수 
    inline static T* instance = nullptr;
};
#pragma once
#pragma warning(disable: 4251)

#include <Windows.h>
#include <comdef.h>
#include <string>

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
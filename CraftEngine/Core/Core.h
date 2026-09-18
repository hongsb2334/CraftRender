#pragma once
#pragma warning(disable: 4251)

#include <Windows.h>
#include <comdef.h>

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

#define ThrowIfFailed(result, message)														\
	do																						\
	{																						\
		if (FAILED(result))																	\
		{																					\
			_com_error error(result);														\
			const wchar_t* errorMessage = error.ErrorMessage();								\
			wchar_t buffer[2048] = {};														\
			wsprintf(buffer, L"File: %s\nFunction: %s\nLine: %d\nMessage: %s\nError: %s\n",	\
				GET_FILE_NAME, GET_FUNCTION_NAME, __LINE__, message, errorMessage);			\
			MessageBox(nullptr, buffer, L"D3D Error", MB_OK);								\
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
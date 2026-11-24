#pragma once

#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

//크기가 고정된 일반 데이터 저장용 함수
//POD: Plain Old Data - 일반 데이터 지정
template<typename T>
void WritePod(std::ofstream& output, const T& value)
{
    //파일에 기록
    output.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

//가변 문자열 저장 함수
void WriteString(std::ofstream& output, const std::string& value);

//경로 생성 함수
//원본 경로를 기반으로 임포트 후 파일을 저장할 경로 생성
//확장자: .ctexture / .cmesh
fs::path RelativeOutput(const fs::path& inputRoot, const fs::path& outputRoot, const fs::path& inputPath, const char* extension);

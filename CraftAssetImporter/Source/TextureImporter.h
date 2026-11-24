#pragma once

#include <cstdint>
#include <filesystem>

namespace fs = std::filesystem;

//CTexture 파일 쓰기 함수
bool WriteTextureAsset(const uint8_t* pixels, uint32_t width, uint32_t height, const fs::path& outputPath);

//이미지 원본 파일 (jpg,png) 등을 cTexture 로 변환하는 함수
bool ImportTexture(const fs::path& inputPath, const fs::path& outputPath);
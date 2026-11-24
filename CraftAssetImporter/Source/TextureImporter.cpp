#include "TextureImporter.h"
#include <source/Utility.h>
#include <CraftAssets/AssetTypes.h>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <ThirdParty/stb_image.h>

bool WriteTextureAsset(const uint8_t* pixels, uint32_t width, uint32_t height, const fs::path& outputPath)
{
    //부모 경로의 폴더 생성
    fs::create_directories(outputPath.parent_path());

    //파일 열기 (쓰기모드)
    std::ofstream output(outputPath, std::ios::binary);
    if (!output.is_open())
    {
        std::cerr << "Failed to create texture asset : " << outputPath << "\n";

        return false;
    }

    //에셋 시그니처 기록
    output.write(Craft::cTextureMagic, sizeof(Craft::cTextureMagic));

    //에셋 버전 기록
    WritePod(output, Craft::assetVersion);

    //텍스처 에셋 데이터 기록
    const uint32_t channels = 4;
    WritePod(output, channels);

    //픽셀 데이터 배열 기록
    //배열 전체 바이트 크기
    const auto byteCount = static_cast<std::streamsize>(width * height * channels);

    //픽셀 데이터를 파일에 기록
    output.write(reinterpret_cast<const char*>(pixels), byteCount);

    return true;
}

bool ImportTexture(const fs::path& inputPath, const fs::path& outputPath)
{
    int width = 0;
    int height = 0;
    int sourcechannels = 0;

    //이미지 로드해서 픽셀 데이터(배열) 저장
    stbi_uc* pixels = stbi_load(inputPath.string().c_str(), &width, &height, &sourcechannels, STBI_rgb_alpha);

    //pixels == nullptr이면 로드 실패
    if (pixels == nullptr)
    {
        std::cerr << "failed to import texture" << inputPath << "\n";
        return false;
    }

    const bool succeeded = WriteTextureAsset(pixels, static_cast<uint32_t>(width), static_cast<uint32_t>(height), outputPath);

    //로드한 이미지 배열 해제
    stbi_image_free(pixels);

    //성공한 경우에는 성공 메시지 출력
    if (succeeded)
    {
        std::cout << "Imported texture: " << inputPath << " -> " << outputPath << "\n";
    }

    return succeeded;
}

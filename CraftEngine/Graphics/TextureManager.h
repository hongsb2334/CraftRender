#pragma once
#include <Core/Core.h>
#include <memory>
#include <string>
#include <unordered_map>


namespace Craft
{
    //전방 선언
    class Texture;

    class TextureManager : public TGlobalAccessible<TextureManager>
    {
    public:
        TextureManager() = default;
        ~TextureManager() = default;

        //텍스처 로드 (또는 반환) 함수
        std::shared_ptr<Texture> LoadTexture(const std::string& path);
    private:
        //관리하는 텍스처 객체 (키: 파일 경로)
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

    };
}



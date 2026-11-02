#include "TextureManager.h"
#include "Texture.h"
#include <Windows.h>

namespace Craft
{
    std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& path)
    {
        // 경로 값으로 텍스처 검색
        auto result = textures.find(path);

        // unordered_map 의 find 함수는 검색에 실패하면 end iterator 반환
        if (result != textures.end())
        {
            //성공한 경우에는 값 반환
            return result->second;
        }

        //검색 실패한 경우에는 텍스처가 로드된 상태가 아니기 때문에
        //텍스처를 생성한 뒤에 맵에 추가해주고 반환

        auto newTexture = Texture::Load(path);
        textures.emplace(path, newTexture);
        return newTexture;
    }
}

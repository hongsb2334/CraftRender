#include "ShaderManager.h"
#include "Shader.h"

namespace Craft
{
    std::shared_ptr<Shader> ShaderManager::LoadShader(const std::wstring& name)
    {
        // 이름 값으로 셰이더 검색
        auto result = shaders.find(name);

        // unordered_map 의 find 함수는 검색에 실패하면 end iterator 반환
        if (result != shaders.end())
        {
            //성공한 경우에는 값 반환
            return result->second;
        }

        //검색 실패한 경우에는 셰이더가 로드된 상태가 아니기 때문에
        //셰이더를 생성한 뒤에 맵에 추가해주고 반환
        auto newShader = std::make_shared<Shader>(name);
        shaders.emplace(name, newShader);
        return newShader;
    }
}
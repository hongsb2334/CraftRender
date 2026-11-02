#include "Material.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "TextureManager.h"

namespace Craft
{
    DefaultMaterial::DefaultMaterial() : shader(ShaderManager::Get().LoadShader(L"Default"))
    {
        
    }
    void DefaultMaterial::Bind() const
    {
        //셰이더 바인딩
        shader->Bind();

    }
    TextureMappingMaterial::TextureMappingMaterial(const std::string& texturePath) : shader(ShaderManager::Get().LoadShader(L"TextureMapping")),
        texture(TextureManager::Get().LoadTexture(texturePath))
    {
    }
    void TextureMappingMaterial::Bind() const
    {
        shader->Bind();
        texture->Bind(0);
    }
}
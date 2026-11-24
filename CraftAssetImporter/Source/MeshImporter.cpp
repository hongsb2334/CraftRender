#include "MeshImporter.h"
#include "TextureImporter.h"
#include "Utility.h"

#include <fstream>
#include <iostream>

bool WriteMeshAsset(const Craft::MeshAsset& mesh, const fs::path& outputPath)
{
    /*
    //정점 배열 정보
    std::vector<AssetVertex> vertices;

    //인덱스 배열 정보
    std::vector<uint32_t> indices;

    // 서브메시 배열 정보
    std::vector<AssetSubMesh> subMeshes;
    */

    //상위 폴더 생성
    fs::create_directories(outputPath.parent_path());

    //출력 용도로 파일 스트림 생성
    std::ofstream output(outputPath, std::ios::binary);

    if (!output.is_open())
    {
        std::cerr << "Failed to create mesh asset" << outputPath << "\n";

        return false;
    }

    //시그니처 기록
    output.write(Craft::cMeshMagic, sizeof(Craft::cMeshMagic));

    //에셋 버전 기록
    WritePod(output, Craft::assetVersion);

    //메시 에셋 데이터 기록
    const uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
    const uint32_t indexCount = static_cast<uint32_t>(mesh.indices.size());

    WritePod(output, vertexCount);
    WritePod(output, indexCount);

    //정점 배열 데이터 기록
    output.write(reinterpret_cast<const char*>(mesh.vertices.data()), sizeof(Craft::AssetVertex) * vertexCount);

    //인덱스 배열 데이터 기록
    output.write(reinterpret_cast<const char*>(mesh.indices.data()), sizeof(uint32_t) * vertexCount);

    //서브메시 데이터 기록

    return false;
}

bool ImportMesh(const fs::path& inputPath, const fs::path& outputPath, const fs::path& outputRoot)
{
    return false;
}

std::string ImportDiffuseTexture(const aiScene* scene, const aiMesh* sourceMesh, const fs::path& meshSourcePath, const fs::path& outputRoot, const fs::path& meshOutputPath)
{
    return std::string();
}

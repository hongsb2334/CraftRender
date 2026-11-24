#include <Source/TextureImporter.h>
#include <Source/Utility.h>

#include <filesystem>
#include <iostream>

//네임스페이스 재정의
namespace fs = std::filesystem;

//텍스처 파일인지 확인하는 함수
bool IsTexture(const std::string& extension)
{
    return extension == ".tga" || extension == ".targa" || extension == ".jpg" || extension == ".jpeg" || extension == ".png";
}

int main(int argc, char** argv)
{
    //실행 명령어 포함 실행 인자 수가 3개인지 확인
    if (argc < 3)
    {
        std::cout << "Usage : CraftAssetImporter <source_dir> <output_dir>\n";

        return 1;
    }

    //원본 에셋 경로
    fs::path sourceRoot = argv[1];

    //임포트 처리 후에 저장하는 경로
    fs::path outputRoot = argv[2];
    
    //오류 확인 - 원본 경로가 존재하는지 확인
    if (!fs::exists(sourceRoot))
    {
        std::cerr << "Source directory does not exist : " << sourceRoot << "\n";

        return 2;
    }
    

    //경로 출력
    std::cout << "Source    : " << sourceRoot << "\n";
    std::cout << "Imported  : " << outputRoot << "\n";

    //임포트 처리 확인
    bool succeeded = true;


    //원본 경로로 이동해서 파일 순회 처리
    for (const auto& entry : fs::recursive_directory_iterator(sourceRoot))
    {
        //파일이 아니면(폴더 등) 건너뜀
        if (!entry.is_regular_file())
        {
            continue;
        }

        //확인용으로 파일 이름 출력
        //std::cout << entry.path() << "\n";

        //파일에서 확장자 얻기
        std::string extension = entry.path().extension().string();

        //확장자 문자를 소문자로 변환
        for (char& character : extension)
        {
            character = std::tolower(character);
        }

        //텍스처 타입인지 확인 후 임포트 처리
        if (IsTexture(extension))
        {
            //원본 리소스 경로
            auto inputPath = entry.path();

            //출력 에셋 경로
            auto outputPath = RelativeOutput(sourceRoot, outputRoot, inputPath, ".ctexture");

            //임포트 처리함수 실행
            succeeded &= ImportTexture(inputPath, outputPath);
        }
    }

    return succeeded ? 0 : 3;
}
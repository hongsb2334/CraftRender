@echo off
rem 명령어 실행이 화면에 반복 출력되는 것 방지.

setlocal
rem 배치 파일 안에서 사용된 변수가 시스템에 영향을 주지 않도록 처리.

set "ROOT_DIR=%~dp0.."
rem %~dp0은 현재 배치 파일이 위치한 폴더 경로.
rem ..이 붙었기 때문에 현재 경로보다 하나 위 경로를 나타냄.

set "SOURCE_DIR=%ROOT_DIR%\Assets\Source"
rem SOURCE_DIR 변수를 선언하면서 위에서 선언한 ROOT_DIR의 Assets\Source 경로를 설정함.

set "OUTPUT_DIR=%ROOT_DIR%\Assets\Imported"
rem OUTPUT_DIR 변수를 선언하면서 위에서 선언한 ROOT_DIR의 Assets\Imported 경로를 설정함.

set "IMPORTER=%ROOT_DIR%\Bin\Debug\CraftAssetImporter.exe"
rem IMPORTER 변수를 선언하면서 빌드된 AssetImporter의 실행 파일을 설정.
rem 실제 임포터 처리 작업을 실행할 프로그램 경로를 지정한 것.

if not "%~1"=="" set "SOURCE_DIR=%~1"
rem 배치 파일을 실행할 때 첫 번째 인자(파라미터)를 넘겨준 경우 해당 값을 SOURCE_DIR에 사용.

if not "%~2"=="" set "OUTPUT_DIR=%~2"
rem 배치 파일을 실행할 때 두 번째 인자(파라미터)를 넘겨준 경우 해당 값을 OUTPUT_DIR에 사용.

if not exist "%IMPORTER%" (
    echo ERROR: CraftAssetImporter.exe was not found.
    echo Build CraftAssetImporter project first.
    exit /b 1
)
rem IMPORTER 경로에 파일이 없는 경우 오류 출력 후 종료.
rem exit /b 1 -> 프로그램 종료 시 실패 코드 1을 남기고 종료.

"%IMPORTER%" "%SOURCE_DIR%" "%OUTPUT_DIR%"
rem CraftAssetImporter를 실행.

exit /b %errorlevel%
rem CraftAssetImporter를 실행하면서 프로그램에서 반환 받은
rem 결과 코드(%errorlevel%)를 그대로 반환하면서 종료.
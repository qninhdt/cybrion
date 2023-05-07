@echo off
setlocal
set BASE_PATH=%~dp0


set "vswhere=C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%vswhere%" (
    echo Error: vswhere.exe not found.
    exit /b 1
)

for /f "tokens=*" %%v in ('"%vswhere%" -latest -property installationVersion') do set "vsver=%%v"
if "%vsver%" == "" (
    echo Error: Visual Studio not found.
    exit /b 1
)

set "cmakegen="
if "%vsver:~,2%" == "12" (
    set "cmakegen=Visual Studio 12 2013"
) else if "%vsver:~,2%" == "14" (
    set "cmakegen=Visual Studio 14 2015"
) else if "%vsver:~,2%" == "15" (
    set "cmakegen=Visual Studio 15 2017"
) else if "%vsver:~,2%" == "16" (
    set "cmakegen=Visual Studio 16 2019"
) else if "%vsver:~,2%" == "17" (
    set "cmakegen=Visual Studio 17 2022"
) else (
    echo Error: Unsupported Visual Studio version: %vsver%
    exit /b 1
)

echo Detected Visual Studio version: %vsver%
echo Using CMake generator: "%cmakegen%"


REM Install vcpkg

set VCPKG_ROOT=%BASE_PATH%tools\vcpkg

if not exist %VCPKG_ROOT% (
	git clone https://github.com/microsoft/vcpkg.git %VCPKG_ROOT%
	"%VCPKG_ROOT%\bootstrap-vcpkg.bat" -disableMetrics
	echo Vcpkg has been installed successfully.
	"%VCPKG_ROOT%\vcpkg.exe" install ^
		sdl2 ^
        glm ^
        lz4 ^
        robin-hood-hashing ^
        bshoshany-thread-pool ^
		sdlog ^
        yaml-cpp ^
        imgui[opengl3-binding,sdl2-binding] ^
        concurrentqueue ^
		--triplet=x64-windows
) else (
  echo Vcpkg is already installed at %VCPKG_ROOT%. Skipping installation.
)

cmake ^
	-G "%cmakegen%" ^
	-DCMAKE_TOOLCHAIN_FILE=%BASE_PATH%tools\vcpkg\scripts\buildsystems\vcpkg.cmake ^
	-B%BASE_PATH%out
	
cmake --build out --config Release

set MSVC_BUILD_PATH=%BASE_PATH%out\Release
set BUILD_PATH=%BASE_PATH%build

rem Remove old files
if exist %BUILD_PATH% rd /s /q %BUILD_PATH%

mkdir %BUILD_PATH%

echo Copy execute files and resource files ...

rem Copy execute files
xcopy %MSVC_BUILD_PATH%\*.dll %BUILD_PATH% /E /C /I /Y > nul
xcopy %MSVC_BUILD_PATH%\*.exe %BUILD_PATH% /E /C /I /Y > nul

rem Copy resources
xcopy %BASE_PATH%resources %BUILD_PATH%\resources\ /E /C /I /Y > nul

echo Build completed

endlocal

pause
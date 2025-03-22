@echo off
cls

set "EXE_NAME=tests"
set "DESTINATION_DIR=..\build\tests"

IF NOT EXIST ..\build mkdir ..\build
IF NOT EXIST "%DESTINATION_DIR%" mkdir "%DESTINATION_DIR%"

if not defined DevEnvDir (call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat")

if "%Platform%" neq "x64" (
    echo ERROR: Platform is not "x64" - previous bat call failed.
    exit /b 1
)

cd "%DESTINATION_DIR%"
del *.pdb > NUL 2> NUL
del *.idb > NUL 2> NUL
cd ..\..\GameEngine

REM Use /showIncludes for include debugging

set BUILD_TYPE=DEBUG
set BUILD_FLAGS=/Od /Z7 /WX /FC /RTC1 /DDEBUG

set "DEBUG_DATA=/Fd"%DESTINATION_DIR%\%EXE_NAME%.pdb" /Fm"%DESTINATION_DIR%\%EXE_NAME%.map""

REM Parse command-line arguments
if "%1"=="-r" (
    set BUILD_TYPE=RELEASE
    set BUILD_FLAGS=/O2 /D NDEBUG

    set DEBUG_DATA=
)
if "%1"=="-d" (
    set BUILD_TYPE=DEBUG
    set BUILD_FLAGS=/Od /Z7 /WX /FC /RTC1 /DDEBUG

    set "DEBUG_DATA=/Fd"%DESTINATION_DIR%\%EXE_NAME%.pdb" /Fm"%DESTINATION_DIR%\%EXE_NAME%.map""
)

REM Find all .cpp files in the current directory and subdirectories
for /R %%f in (*Test.cpp) do (
    REM Get the base name of the file without extension
    set "BASENAME=%%~nf"

    REM Compile each .cpp file into an executable
    cl ^
        %BUILD_FLAGS% /MT /nologo /Gm- /GR- /EHsc /W4 /wd4201 /wd4706 /wd4324 ^
        /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /std:c++20 ^
        /D WIN32 /D _WINDOWS /D _UNICODE /D UNICODE ^
        /D _CRT_SECURE_NO_WARNINGS ^
        /Fo"%DESTINATION_DIR%/" /Fe"%DESTINATION_DIR%\%BASENAME%.exe" %DEBUG_DATA% ^
        "%%f" ^
        /link /INCREMENTAL:no ^
        /SUBSYSTEM:CONSOLE /MACHINE:X64 ^
        kernel32.lib user32.lib gdi32.lib winmm.lib

    REM Check if the compilation was successful
    if errorlevel 1 (
        echo Compilation failed for %%f
        exit /b 1
    )

    REM Run the compiled executable
    "%DESTINATION_DIR%\%BASENAME%.exe"
)

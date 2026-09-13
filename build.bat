@echo off
rem TitanBrawl36 — сборка под Windows (MinGW + CMake + Ninja).
rem Использование: build.bat [clean] [debug]
setlocal
cd /d "%~dp0"

set BUILD_TYPE=Release
for %%A in (%*) do (
    if /i "%%A"=="debug" set BUILD_TYPE=Debug
    if /i "%%A"=="clean" (
        echo [build] clean...
        if exist build rmdir /s /q build
    )
)

where ninja >nul 2>nul
if %ERRORLEVEL%==0 (
    set GENERATOR=-G Ninja
) else (
    set GENERATOR=
)

echo [build] configure (%BUILD_TYPE%)...
cmake -B build -S . %GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if %ERRORLEVEL% neq 0 (
    echo [build] ОШИБКА: cmake configure упал. Проверь, что стоят CMake и MinGW.
    exit /b 1
)

echo [build] compile...
cmake --build build
if %ERRORLEVEL% neq 0 (
    echo [build] ОШИБКА: сборка упала, смотри лог выше.
    exit /b 1
)

echo [build] OK: build\titan_pc.exe

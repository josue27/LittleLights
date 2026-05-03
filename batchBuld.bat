@echo off
set ENGINE_PATH=E:\Epic\UE_5.4
set PROJECT_PATH=F:\ParallelsGames\LittleLights 5.0\LittleLights.uproject
set OUTPUT_PATH=F:\ParallelsGames\LittleLightsBuilds
set WINRAR_PATH=C:\Program Files\WinRAR\WinRAR.exe
set RAR_OUTPUT=F:\ParallelsGames\LittleLightsBuilds\LittleLights_%date:~-4%-%date:~3,2%-%date:~0,2%.rar

:: Build
"%ENGINE_PATH%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun ^
    -project="%PROJECT_PATH%" ^
    -noP4 ^
    -platform=Win64 ^
    -clientconfig=Shipping ^
    -cook ^
    -build ^
    -stage ^
    -pak ^
    -archive ^
    -archivedirectory="%OUTPUT_PATH%"

:: Verificar build exitoso
if %ERRORLEVEL% NEQ 0 (
    echo BUILD FALLIDO - No se comprimira
    pause
    exit /b 1
)

:: Comprimir
echo Comprimiendo build...
"%WINRAR_PATH%" a -r "%RAR_OUTPUT%" "%OUTPUT_PATH%\*"

if %ERRORLEVEL% EQU 0 (
    echo Listo: %RAR_OUTPUT%
) else (
    echo Error al comprimir
)

pause
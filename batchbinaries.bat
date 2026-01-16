echo on


set MyUVS="C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\Win64\UnrealVersionSelector.exe"
set MyUBT="C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"

rem change Transformation to your own project name
d:
set MyFullPath="%cd%\LittleLights"
set MyFullPath="C:\Users\meval\OneDrive\Documents\ParallelsGames\LittleLights"
pause

%MyUVS% /projectfiles %MyFullPath%.uproject

%MyUBT% Development Win64 -Project=%MyFullPath%.uproject -TargetType=Editor -Progress -NoEngineChanges -NoHotReloadFromIDE

pause

%MyFullPath%.uproject

%MyFullPath%.sln



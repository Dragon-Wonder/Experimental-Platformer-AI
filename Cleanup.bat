@echo off
:: This batch file will go through the folders and delete files that are not needed.
:: Once again really just for personal use
:: There are a lot of random files that get throw about, this batch file will delete them if they are there.
:: because I'm too lazy to go searching for them myself, but I want to keep my folders clean.

echo Cleaning Debug Folder...
IF EXIST %~dp0Debug\build.rasm.log del /F %~dp0Debug\build.rasm.log
IF EXIST %~dp0Debug\PlatformerExperiment-PRIVATE.exe.stackdump del /F %~dp0Debug\PlatformerExperiment-PRIVATE.exe.stackdump
echo.

echo Cleaning Project Folder...
IF EXIST %~dp0Project\bin RMDIR /S /Q %~dp0Project\bin
IF EXIST %~dp0Project\obj RMDIR /S /Q %~dp0Project\obj
IF EXIST %~dp0Project\Experimental-Platformer-AI.depend del /F %~dp0Project\Experimental-Platformer-AI.depend
IF EXIST %~dp0Project\Experimental-Platformer-AI.layout del /F %~dp0Project\Experimental-Platformer-AI.layout
echo.

echo Cleaning Resources Folder...
IF EXIST %~dp0Resource\boilerplate.res del /F %~dp0Resource\boilerplate.res
echo.

echo Cleaning Source Folder...
IF EXIST %~dp0Source\main.o del /F %~dp0Source\main.o
IF EXIST %~dp0Source\config.o del /F %~dp0Source\config.o
IF EXIST %~dp0Source\map.o del /F %~dp0Source\map.o
IF EXIST %~dp0Source\entity.o del /F %~dp0Source\entity.o
IF EXIST %~dp0Source\tick.o del /F %~dp0Source\tick.o
IF EXIST %~dp0Source\screen.o del /F %~dp0Source\screen.o
echo.

echo Cleaning Main Folder...
IF EXIST %~dp0Config.ini del /F %~dp0Config.ini
IF EXIST %~dp0gmon.out del /F %~dp0gmon.out
IF EXIST %~dp0Images.rar del /F %~dp0Images.rar
IF EXIST %~dp0Images.zip del /F %~dp0Images.zip
::IF EXIST %~dp0PlatformerExperiment.exe del /F %~dp0PlatformerExperiment.exe
IF EXIST %~dp0PlatformerExperiment-PRIVATE.exe del /F %~dp0PlatformerExperiment-PRIVATE.exe
IF EXIST %~dp0PlatformerExperiment del /F %~dp0PlatformerExperiment
IF EXIST %~dp0PlatformerExperiment-PRIVATE del /F %~dp0PlatformerExperiment-PRIVATE
IF EXIST %~dp0Player.log del /F %~dp0Player.log
echo.

echo All cleaning done.


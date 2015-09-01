@echo off
:: A few quick notes about this. 
:: %~dp0 is the directory the batch file is in, and it ends with a '\' so don't add it
:: The Batch file has to stay in the main to work as it is written right now

:: This will create the debug verison of the program 
:: I recommend using Code::Blocks instead but this is here is it is needed

echo Deleting old files...
::Delete the old .exe if it exists
IF EXIST PlatformerExperiment-PRIVATE.exe del /F PlatformerExperiment-PRIVATE.exe
::Delete Stackdump if it exists (so as to not confuse me later)
IF EXIST PlatformerExperiment-PRIVATE.exe.stackdump del /F PlatformerExperiment-PRIVATE.exe.stackdump
echo.

::Move to the Resources directory
cd %~dp0Resources
echo Compiling Resources...
::Compile the resource files of the icon and boilerplate
windres boilerplate.rc -DDEFINED_BUILD_MODE_PRIVATE -O coff boilerplate.res
echo.


::Move to the source folder
cd %~dp0Source
echo Compiling source files...
::Complie each of the cpp files
echo Compiling main.cpp...
g++ -std=c++11 -I..\SDL2Stuff\include -Wall -Wextra -g -pg -DDEFINED_BUILD_MODE_PRIVATE -c main.cpp

echo Compiling config.cpp...
g++ -std=c++11 -I..\SDL2Stuff\include -Wall -Wextra -g -pg -DDEFINED_BUILD_MODE_PRIVATE -c config.cpp

echo Compiling map.cpp...
g++ -std=c++11 -I..\SDL2Stuff\include -Wall -Wextra -g -pg -DDEFINED_BUILD_MODE_PRIVATE -c map.cpp

echo Compiling entity.cpp...
g++ -std=c++11 -I..\SDL2Stuff\include -Wall -Wextra -g -pg -DDEFINED_BUILD_MODE_PRIVATE -c entity.cpp

echo Compiling tick.cpp...
g++ -std=c++11 -I..\SDL2Stuff\include -Wall -Wextra -g -pg -DDEFINED_BUILD_MODE_PRIVATE -c tick.cpp

echo Compiling screen.cpp...
g++ -std=c++11 -I..\SDL2Stuff\include -Wall -Wextra -g -pg -DDEFINED_BUILD_MODE_PRIVATE -c screen.cpp

::Move back to the main directory
cd %~dp0

echo.

::Complie everything together!
echo Linking everything together...
g++ -std=c++11 -L"C:\Program Files (x86)\CodeBlocks\MinGW\lib" -L%~dp0SDL2Stuff\lib -Wall -Wextra -g -DDEFINED_BUILD_MODE_PRIVATE -o PlatformerExperiment-PRIVATE.exe %~dp0Source\main.o %~dp0source\config.o %~dp0source\map.o %~dp0source\entity.o %~dp0source\tick.o %~dp0source\screen.o %~dp0Resources\boilerplate.res -pg -lgmon -lmingw32 -lSDL2main -lSDL2

echo. 
::Delete all the leftover parts
echo Deleting object files...
IF EXIST %~dp0Resources\boilerplate.res del /F %~dp0Resources\boilerplate.res
IF EXIST %~dp0Source\main.o del /F %~dp0Source\main.o
IF EXIST %~dp0Source\config.o del /F %~dp0Source\config.o
IF EXIST %~dp0Source\map.o del /F %~dp0Source\map.o
IF EXIST %~dp0Source\entity.o del /F %~dp0Source\entity.o
IF EXIST %~dp0Source\tick.o del /F %~dp0Source\tick.o
IF EXIST %~dp0Source\screen.o del /F %~dp0Source\screen.o

echo. 

echo Done!
::Pause encase there was a complie error to allow user to see what the issue is.
pause

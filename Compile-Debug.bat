@echo off
:: A few quick notes about this. 
:: %~dp0 is the directory the batch file is in, and it ends with a '\' so don't add it
:: The Batch file has to stay in the main to work as it is written right now

:: This will create the debug verison of the program 
:: I recommend using Code::Blocks instead but this is here is it is needed

echo Deleting old files...
::Delete the old .exe if it exists
IF EXIST Platformer-Experiment-PRIVATE.exe del /F Platformer-Experiment-PRIVATE.exe
::Delete Stackdump if it exists (so as to not confuse me later)
IF EXIST Platformer-Experiment-PRIVATE.exe.stackdump del /F Platformer-Experiment-PRIVATE.exe.stackdump
echo.

::Move to the Resources directory
cd %~dp0Resources
echo Compiling Resources...
::Compile the resource files of the icon and boilerplate
windres my_icon.rc -O coff my_icon.res
windres boilerplate.rc -O coff boilerplate.res
echo.


::Move to the source folder
cd %~dp0Source
echo Compiling source files...
::Complie each of the cpp files
echo Compiling main.cpp...
g++ -std=c++11 -Wall -Wextra -g -pg -c main.cpp

echo Compiling config.cpp...
g++ -std=c++11 -Wall -Wextra -g -pg -c config.cpp

echo Compiling map.cpp...
g++ -std=c++11 -Wall -Wextra -g -pg -c map.cpp

echo Compiling entity.cpp...
g++ -std=c++11 -Wall -Wextra -g -pg -c entity.cpp

echo Compiling tick.cpp...
g++ -std=c++11 -Wall -Wextra -g -pg -c tick.cpp

::Move back to the main directory
cd %~dp0

echo.

::Complie everything together!
echo Linking everything together...
g++ -std=c++11 -Wall -Wextra -g -pg -o Platformer-Experiment-PRIVATE.exe %~dp0Source\main.o %~dp0source\config.o %~dp0source\map.o %~dp0source\entity.o %~dp0source\tick.o %~dp0Resources\my_icon.res %~dp0Resources\boilerplate.res

echo. 
::Delete all the leftover parts
echo Deleting object files...
IF EXIST %~dp0Resources\boilerplate.res del /F %~dp0Resources\boilerplate.res
IF EXIST %~dp0Resources\my_icon.res del /F %~dp0Resources\my_icon.res
IF EXIST %~dp0Source\main.o del /F %~dp0Source\main.o
IF EXIST %~dp0Source\config.o del /F %~dp0Source\config.o
IF EXIST %~dp0Source\map.o del /F %~dp0Source\map.o
IF EXIST %~dp0Source\entity.o del /F %~dp0Source\entity.o
IF EXIST %~dp0Source\tick.o del /F %~dp0Source\tick.o

echo. 

echo Done!
::Pause encase there was a complie error to allow user to see what the issue is.
pause

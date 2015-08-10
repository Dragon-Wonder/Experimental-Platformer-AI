@echo off
:: A few quick notes about this. 
:: %~dp0 is the directory the batch file is in, and it ends with a '\' so don't add it
:: The Batch file has to stay in the main to work as it is written right now

::Delete the old .exe if it exists
IF EXIST Platformer-Experiment.exe del /F Platformer-Experiment.exe
::Delete Stackdump if it exists (so as to not confuse me later)
IF EXIST Platformer-Experiment.exe.stackdump del /F Platformer-Experiment.exe.stackdump
::Move to the Resources directory
cd %~dp0Resources
::Complie the resource files of the icon and boilerplate
windres my_icon.rc -O coff my_icon.res
windres boilerplate.rc -O coff boilerplate.res
::Move to the source folder
cd %~dp0Source
::Complie each of the cpp files
g++ -std=c++11 -Wall -g -c main.cpp
g++ -std=c++11 -Wall -g -c config.cpp
g++ -std=c++11 -Wall -g -c map.cpp
g++ -std=c++11 -Wall -g -c entity.cpp
g++ -std=c++11 -Wall -g -c tick.cpp
::Move back to the main directory
cd %~dp0
::Complie everything together!
g++ -std=c++11 -Wall -g -o Platformer-Experiment.exe %~dp0Source\main.o %~dp0source\config.o %~dp0source\map.o %~dp0source\entity.o %~dp0source\tick.o %~dp0Resources\my_icon.res %~dp0Resources\boilerplate.res
::Delete all the leftover parts
IF EXIST %~dp0Resources\boilerplate.res del /F %~dp0Resources\boilerplate.res
IF EXIST %~dp0Resources\my_icon.res del /F %~dp0Resources\my_icon.res
IF EXIST %~dp0Source\main.o del /F %~dp0Source\main.o
IF EXIST %~dp0Source\config.o del /F %~dp0Source\config.o
IF EXIST %~dp0Source\map.o del /F %~dp0Source\map.o
IF EXIST %~dp0Source\entity.o del /F %~dp0Source\entity.o
IF EXIST %~dp0Source\tick.o del /F %~dp0Source\tick.o
::Pause encase there was a complie error to allow user to see what the issue is.
pause
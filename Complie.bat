@echo off
del Platformer-Experiment.exe
cd %~dp0Resources\
windres my_icon.rc -O coff my_icon.res
windres boilerplate.rc -O coff boilerplate.res
cd %~dp0Source
g++ -std=c++11 -Wall -g -c main.cpp > errors.log
g++ -std=c++11 -Wall -g -c config.cpp > errors.log
g++ -std=c++11 -Wall -g -c map.cpp > errors.log
g++ -std=c++11 -Wall -g -c entity.cpp > errors.log
g++ -std=c++11 -Wall -g -c tick.cpp > errors.log
cd %~dp0/
g++ -std=c++11 -Wall -g -o Platformer-Experiment.exe %~dp0Source\main.o %~dp0source\config.o %~dp0source\map.o %~dp0source\entity.o %~dp0source\tick.o %~dp0Resources\my_icon.res %~dp0Resources\boilerplate.res > errors.log
del %~dp0/Resources/boilerplate.res
del %~dp0/Resources/my_icon.res
del %~dp0/Source/main.o
del %~dp0/Source/config.o
del %~dp0/Source/map.o
del %~dp0/Source/entity.o
del %~dp0/Source/tick.o

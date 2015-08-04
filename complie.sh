rm marioexperiment.exe
cd ./Resources/
windres my_icon.rc -O coff my_icon.res
windres boilerplate.rc -O coff boilerplate.res
cd ../
g++ -Wall -g -c mario.cpp
g++ -g -o marioexperiment.exe mario.o -lncurses ./Resources/my_icon.res ./Resources/boilerplate.res
rm ./Resources/boilerplate.res
rm ./Resources/my_icon.res
rm mario.o

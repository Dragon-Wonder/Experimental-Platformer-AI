rm marioexperiment.exe
cd ./Resources/
windres my_icon.rc -O coff my_icon.res
windres boilerplate.rc -O coff boilerplate.res
cd ../
g++ -Wall -g -c main.cpp
g++ -g -o marioexperiment.exe main.o ./Resources/my_icon.res ./Resources/boilerplate.res
rm ./Resources/boilerplate.res
rm ./Resources/my_icon.res
rm main.o

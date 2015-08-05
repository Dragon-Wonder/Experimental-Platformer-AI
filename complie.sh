rm Platformer-Experiment.exe
cd ./Resources/
windres my_icon.rc -O coff my_icon.res
windres boilerplate.rc -O coff boilerplate.res
cd ../Source/
g++ -Wall -g -c platformer.cpp
g++ -Wall -g -c config.cpp
cd ../
g++ -g -o Platformer-Experiment.exe ./Source/platformer.o ./source/config.o ./Resources/my_icon.res ./Resources/boilerplate.res
rm ./Resources/boilerplate.res
rm ./Resources/my_icon.res
rm ./Source/platformer.o
rm ./Source/config.o

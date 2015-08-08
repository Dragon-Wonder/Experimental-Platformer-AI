rm Platformer-Experiment.exe
cd ./Resources/
windres my_icon.rc -O coff my_icon.res
windres boilerplate.rc -O coff boilerplate.res
cd ../Source/
g++ -std=c++11 -Wall -g -c main.cpp > errors.txt
g++ -std=c++11 -Wall -g -c config.cpp > errors.txt
g++ -std=c++11 -Wall -g -c map.cpp > errors.txt
g++ -std=c++11 -Wall -g -c entity.cpp > errors.txt
cd ../
g++ -std=c++11 -Wall -g -o Platformer-Experiment.exe ./Source/main.o ./source/config.o ./source/map.o ./source/entity.o ./Resources/my_icon.res ./Resources/boilerplate.res > errors.txt
rm ./Resources/boilerplate.res
rm ./Resources/my_icon.res
rm ./Source/main.o
rm ./Source/config.o
rm ./Source/map.o
rm ./Source/entity.o
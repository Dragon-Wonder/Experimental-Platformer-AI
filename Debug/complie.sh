#Legacy complie script, used for debugging purposes with Cygwin.

#Move out of Debug directory
cd ../
#Delete the old .exe if it exists
rm Platformer-Experiment.exe
#Move to the Resources directory
cd ./Resources/
#Complie the resource files of the icon and boilerplate
windres my_icon.rc -O coff my_icon.res
windres boilerplate.rc -O coff boilerplate.res
#Move to the source folder
cd ../Source
#Complie each of the cpp files
g++ -std=c++11 -Wall -g -c main.cpp
g++ -std=c++11 -Wall -g -c config.cpp
g++ -std=c++11 -Wall -g -c map.cpp
g++ -std=c++11 -Wall -g -c entity.cpp
g++ -std=c++11 -Wall -g -c tick.cpp
#Move back to the main directory
cd ../
#Complie everything together!
g++ -std=c++11 -Wall -g -o Platformer-Experiment.exe ./Source/main.o ./Source/config.o ./Source/map.o ./Source/entity.o ./Source/tick.o ./Resources/my_icon.res ./Resources/boilerplate.res
#Delete all the leftover parts
rm ./Resources/boilerplate.res 
rm ./Resources/my_icon.res 
rm ./Source/main.o 
rm ./Source/config.o 
rm ./Source/map.o 
rm ./Source/entity.o 
rm ./Source/tick.o 

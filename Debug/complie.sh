#Legacy complie script, used for debugging purposes with Cygwin.

#Move out of Debug directory
cd ../
#Delete the old .exe if it exists
rm Platformer-Experiment-PRIVATE.exe
#Move to the Resources directory
cd ./Resources/
#Complie the resource files of the icon and boilerplate
windres boilerplate.rc -O coff boilerplate.res
#Move to the source folder
cd ../Source
#Complie each of the cpp files
g++ -std=c++11 -I../SDL2Stuff/include -Wall -Wextra -pg -g -c main.cpp
g++ -std=c++11 -I../SDL2Stuff/include -Wall -Wextra -pg -g -c config.cpp
g++ -std=c++11 -I../SDL2Stuff/include -Wall -Wextra -pg -g -c map.cpp
g++ -std=c++11 -I../SDL2Stuff/include -Wall -Wextra -pg -g -c entity.cpp
g++ -std=c++11 -I../SDL2Stuff/include -Wall -Wextra -pg -g -c tick.cpp
g++ -std=c++11 -I../SDL2Stuff/include -Wall -Wextra -pg -g -c screen.cpp
#Move back to the main directory
cd ../
#Complie everything together!
g++ -std=c++11 -L./SDL2Stuff/lib -Wall -Wextra -pg -g -o PlatformerExperiment-PRIVATE.exe ./Source/main.o ./Source/config.o ./Source/map.o ./Source/entity.o ./Source/tick.o ./Source/screen.o ./Resources/boilerplate.res -lSDL2main -lSDL2
#Delete all the leftover parts
rm ./Resources/boilerplate.res 
rm ./Source/main.o 
rm ./Source/config.o 
rm ./Source/map.o 
rm ./Source/entity.o 
rm ./Source/tick.o 
rm ./Source/screen.o
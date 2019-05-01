# Experimental Platformer AI

(Check out the gh-pages of this project by going [here](http://dragon-wonder.github.io/Experimental-Platformer-AI/) )

## Introduction

The idea behind this project is a simple AI that will try and "learn" to go through a platforming course. By generating a number of players for a generation. 
For this generation each player is given a set of randomly generated directions that the player will use to go through the course. The further a long the course 
that the "player" gets the more "fit" that player is. The top 10 players in terms of fitness will be "mated" with each other to make a new generation that will be based 
on the previous generations' movements. This repeats for as many times as specified in the config file. The hope is that by the end the "players" will make it to the end of the map.

There is really no point to this entire project its mostly for my friend and I to test the waters in this AI method. As well as improve our coding ability. 


## License and copyright

All of this project was written from scratch by [GamerMan7799](https://github.com/GamerMan7799) & [xPUREx](https://github.com/xPUREx) unless otherwise noted.

That being said all parts of this project we own have been released to the Public Domain. (This does not apply to parts that were noted as being someone else's). 

As such you may use any and/or all of this project however you wish; Commercially or not; modify it or not. You do not even have to give credit (though we would greatly appreciate it if you did).

You can read more about the Public Domain Dedication [Here](http://unlicense.org/).

This project uses libraries created by other individuals. Each one of course has a license by their own owner, these can be found under Documentation/Library Licenses.
Please read these licenses before you decide to distribute any copies of this project.

## Questions, patches, and other feedback

We are pretty novice programmers at best. Part of the reason we made this was to improve our skills. And part of the reason we made it open source is to get feedback.

We are always open to people contributing to the project by giving suggestions, or helping us with improvements. 

If you have any questions or a suggestions or improvement please feel free to leave it as an issue in [this tab](https://github.com/Dragon-Wonder/Experimental-Platformer-AI/issues).

## Building the Project

### SDL Library

In order to run the program properly you will need to download SDL2-2.0.3 or higher. 
You will also need SDL2 TTF library, and SDL2 Images library

A link for SDL2 can be found [here](https://www.libsdl.org/download-2.0.php).

A link for SDL2 TTF can be found [here](https://www.libsdl.org/projects/SDL_ttf/).

A link for SDL2 Images can be found [here](https://www.libsdl.org/projects/SDL_image/).

You will want to download the Runtime Binaries (if you have windows I recommend 32 bit over 64 bit; even if you have a 64 bit computer).

If you are running the version in the release tab you WILL need the 32 bit version because that it how I compile it.

I have also included copies of the 32 bit Runtime binaries in the releases. You can just download these if you don't know what you're doing and you're on Windows.

You will also need to download and extract the Images.zip file provided in the release tab.

When all is said and done you should have the following files in your folder. (Also note that you should be able to put the dll files in your C drive).

```
FolderName\
FolderName\Platformer-Experiment.exe
FolderName\SDL2.dll
FolderName\SDL2_ttf.dll
FolderName\SDL2_image.dll
FolderName\libfreetype-6.dll
FolderName\zlib1.dll
FolderName\libpng16-16.dll
FolderName\Images\tiles.png
```

### Windows

You can download the .exe under the [Releases Tab](https://github.com/Dragon-Wonder/Experimental-Platformer-AI/releases) you will need the SDL2 runtime binaries (see above).

If you wish to compile it yourself you can use the Compile.bat files if you have MinGW on your computer, or you can use the Compile.sh in the Debug folder if you have Cygwin, or you can open the Code Block project file and compile it that way.

Any other complier you wish will work; these are just the ones I've used and made scripts to help with the compiling.
	
### Linux

You should be able to use the Makefile or the Compile.sh script to compile the program. You will also need the SDL library which you can use by enter the following code into your terminal, or downloading from the link above.

Ubuntu, Mint, or Debian

``` 
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-image-dev
```

### IOSX

I don't know you're on your own. Sorry.

## Using the Project

Most of the program will just run by itself because as stated it is meant to be an AI program. When the program is first launched it will create a config file called "Config.ini".
You can change values in this file to change how the program runs. You should read the manual, under documentation.



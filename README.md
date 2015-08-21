# Experimental Platformer AI

## Introduction

The idea behind this project is a simple AI that will try and "learn" to go through a platforming course. By generating a number of players for a generation. 
For this generation each player is given a set of randomly generated directions that the player will use to go through the course. The further a long the course 
that the "player" gets the more "fit" that player is. The top 10 players in terms of fitness will be "mated" with each other to make a new generation that will be based 
on the previous generations' movements. This repeats for as many times as specified in the config file. The hope is that by the end the "players" will make it to the end of the map.

There is really no point to this entire project its mostly for my friend and I to test the waters in this AI method. As well as improve our coding ability. 


## License and copyright

All of this project was written from scratch by [GamerMan7799] (https://github.com/GamerMan7799) & [xPUREx](https://github.com/xPUREx) unless otherwise noted.

That being said all parts of this project we own have been released to the Public Domain. (This does not apply to parts that were noted as being someone's). 

As such you may use any and all of this project however you wish; Commercially or not; modify it or not. You do not even have to give credit (though we would greatly appreciate it if you did).

You can read more about the Public Domain Dedication [Here](http://unlicense.org/).

## Questions, patches, and other feedback

We are pretty novice programmers at best. Part of the reason we made this was to improve our skills. And part of the reason we made it open source is to get feedback.

We are always open to people contributing to the project by giving suggestions, or helping us with improvements. 

If you have any questions or a suggestions or improvement please feel free to leave it as an issue in [this tab] (https://github.com/GamerMan7799/Experimental-Platformer-AI/issues).

## Building the Project

### Windows

You can download the .exe under the [Releases Tab](https://github.com/GamerMan7799/Experimental-Platformer-AI/releases) you should just be able to run this as is (if 4.0.0+; older editions require extra files included in the download).

If you wish to compile it yourself you can use the Compile.bat file if you have MinGW on your computer or you can use the Compile.sh in the Debug folder if you have Cygwin.

Any other complier you wish will work; these are just the ones I've used and made script to help with the compiling.
	
### Linux

You should be able to use the Makefile or the Compile.sh script to compile the program. You shouldn't need any other files.

### IOSX

I don't know you're on your own. Sorry.

## Using the Project

Most of the program will just run by itself because as stated it is meant to be an AI program. When the program is first launched it will create a config file called "Config.ini".
You can change values in this file to change how the program runs. You should read the manual, under documentation.



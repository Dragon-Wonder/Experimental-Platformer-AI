# Change Log

All notable changes to this project will be documented here.
This project adheres to [Semantic Versioning](http://semver.org/)

## UNRELEASED [4.2.0] - 2015-09-11

### Added

* Human inputs for game under certain builds

### Changed

* Player and Monster movement is now velocity based.
* Map

### Fixed




## Old versions

```

## [4.1.3] - 2015-09-11

### Added

* An edge detector (testing the water with it)
* Some states to monsters and players (this is mostly for later versions)

### Changed

* Clock now runs off of seconds instead of ticks
* Screen Loaded Boolens now grouped together
* Colors now grouped together
* Textures now grouped together
* All map tile are now a single image with clipping

### Fixed

* Issue with -s, -Os, or -O3 enabled during complie


## [4.1.2] - 2015-09-04

### Fixed

* VSnyc bug
* Screen being jumpy as the player moves (screen is locked on the first 35 spaces until 
	player goes to the 36th spot then it will be locked on the next 35 spaces, and so on.)
* Font loading issue on Cygwin

### Added

* Player can now to nothing! Wow!

### Changed

* Error image is now embedded so that even if it cannot load images; it will show the error pictures instead.
* If font cannot be loaded, messages won't appear instead of program closing



## [4.1.1] - 2015-09-03

### Fixed

* A bug with screen height being linked to player height causing screen to be a bit strange.

### Note

* In this release I accidently left DEFINED_VER_STATUS as "Alpha" which means that the config file will always be replaced. 


## [4.1.0] - 2015-09-03

### Added

* Debug Compile batch file as well as a final Compile batch
* SDL graphics for show map.
* SDL TTF for text on screen.
* SDL Image for loading pngs.
* Place holder png for map tiles
* Note to ReadMe explaining that you HAVE to use SDL 2.0.4

### Changed

* clsConfig::exists to use the cstdio library instead of fstream
* My_Icon.rc merged into Boilerplate.rc
* Complie batch files updated for changes.
* Default Show Map from false to true
* Makefile with changes.
* Config Version check now also checks software status.
* Player Death anaimation will now show when player runs out of inputs
* Player death is now logged.
* Player can now die from too low fitness in hard mode.
* Tick wait is decreased the longer the rest of the program runs for

### Removed

* Some redundant and pointless code.
* Entity Start (now everything is handled in main.cpp)

### Fixed

* Private build define not properly registering in boilerplate
* Prompt for using old config not working


## [4.0.0] - 2015-08-21

### Notes

* This version has a completely rewritten code; I'll to remember the major things that are different but I may have forgotten stuff.
* The main point of this rewrite was to improve the structure of the code so later changes are easier as opposed to adding a bunch of new features.
* This was one big pain of a rewrite but hopefully it'll make things easier to improve upon in the future.

### Removed

* Generate Random Number Function as it was quite useless
* Windows.h library requirement
* The increase chance of the player with the highest fitness being selected over the other players.

### Changed

* Spilt the program into a lot more .cpp to better organize everything
* Change complier from Cygwin to MinGW
* Changed the icon (same author just different coloring)
* Change log around so that the bullet points appear
* The Read Me so that it is not off putting to the people who find the code. As well as better explain it.

### Added

* Classes
* Tick with universal "sleep"
* More debug messages.
* More comments.
* Makefile for Linux builds.
* Compile.bat for MinGW Windows builds.
* Debug Folder with some debug scripts
* Documentation Folder for documentation
* Count down that will kill player when it reaches 0.
* Define of Best Players
* Naming Notation
* A short death animation

## [3.3.0] - 2015-08-05

### Added

* More comments to (hopefully) better explain stuff

### Changed

* The icon (like 3 times because I can't decide what I want)
* File version numbering changed slightly in boilerplate
	For File Version it goes [MAJOR].[MINOR].[PATCH].[BUILD NUMBER]
	This is mostly just when I am working on the new RC. It won't be
	reflected in the Product Version which will still follow [MAJOR].[MINOR].[PATCH]

* Boilerplate.rc around to make changes to it easier to do.
* Easier to change log name
* Generation number added to log
* At the end of a generation a line is made in the log to help divide them easier
* Log name from "log.txt" to "player.log"
	This so that now that I have GitHub on my computer I can set it
	to ignore *.log rather than *.txt
* How "Mating" is done to make the New generation

### Fixed

* It just occurred to me that the program does not log the last player of a generation
	so I fixed that.


## [3.2.0] - 2015-08-04

### Changed

* Player gets fitness boost for killing a monster
* Monster from "O" to "+"
* Name from Mario to Platformer!
	(Please don't sue me Nintendo!)

### Added

* Coins (are "O")
* Score for player. Goes up after collecting coins, and killing monsters.
	Weighs into fitness (I did this because under hard mode these actions would not effect fitness)


## [3.1.0] - 2015-08-04

### Added

* Multiple jumps allowed; it is configurable
* Configurable Jump height

### Changed

* How player falling is handled.
* Config functions split into its own .cpp
* Changed Monster symbol from '?' to 'O'

### Fixed

* Bug where all fitnesses were displayed as 0 because I was
restarting the map before I recorded the fitness -_-


## [3.0.0] - 2015-08-04

### Changed

* Made several variable defined at the start of the code to make them easier to change.
	These include:
	* Map Height
	* Map Width
	* Max Player Steps (Cannot be greater than 65535)
	* Players per Generation (Cannot be less than 10)
	* Sleep Time
* Mario.h - a basic header to clean up the main code
* Base map moved to Mario.h


## [2.1.0] - 2015-08-03

### Added
* Program now checks version number written to config file with its own
	if the numbers do not match it will overwrite the old file with a new one
	this is to prevent program crashing bugs if I add something to a new config
	that isn't in the old one.

* Added Icon to complied application (will likely change later just testing what I can do.)

### Changed

* A player "jumping" on a monster will kill it.


## [2.0.0] - 2015-08-01

### Added

* "Hard Mode" in config
* With this enabled players die under more conditions

### Changed

* Fitness drop from frames increased.
* Code performance increased (removed unneeded for loops)
* Map up a bit to increase the chances of a player making it through

### Removed

* A lot of debugging lines I commented out


## [1.2.1] - 2015-08-01

### Fixed

* A misplaced check of if the map should be shown that crashed the program



## [1.2.0] - 2015-08-01 - [YANKED]

### Added

* Version Number added to Config file

### Fixed

* Monster movement is no longer randomly move.
	Hopefully to improve survivability between generations



## [1.1.2] - 2015-08-01 - [YANKED]

### Fixed

* Best Player list no longer appears on show map update



## [1.1.1] - 2015-08-01

### Fixed

* Increase wait delay from 20 to 65 so updating is not so jarring.



## [1.1.0] - 2015-07-31

### Added

* Wait between show map: 20 ms

### Changed

* Map no longer waits for pause between EVERY update.

### Fixed

* A monster walking into a player will kill them
* A player attempting to jump with a block two spaces above them will only jump 1 block
* Player can no longer jump through a 1 block high platform
* Generation counter is fixed
* Certain areas of the map that were impassable



## [1.0.0] - 2015-07-31

### Added

* Monsters that move randomly



## [0.9.0] - 2015-07-29

### Added

* Change Log
* Config file so I don't have to keep recompiling to test stuff

### Notes

* Original

```

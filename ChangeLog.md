# Change Log

All notable changes to this project will be documented here.
This project adheres to [Semantic Versioning](http://semver.org/)

## UNRELEASED [3.3.0] - 2015-08-05

### Changed
	
	* The icon
	
	* File version numbering changed slightly in boilerplate
		For File Version it goes [MAJOR].[MINOR].[PATCH].[BUILD NUMBER]
		This is mostly just when I am working on the new rc. It won't be
		reflected in the Product Verison which will still follow [MAJOR].[MINOR].[PATCH]
		
	* Boilerplate.rc around to make changes to it easier to do.
	
	* Easier to change log name
	
	* Generation number added to log
	
	* At the end of a generation a line is made in the log to help divide them easier

## [3.2.0] - 2015-08-04

### Changed

	* Player gets fitness boost for killing a monster
	
	* Monster from "O" to "+"
	
	* Name from Mario to Platformer!
		(Please don't sue me Nintendo!)
	
### Added

	* Coins (are "O")
	
	* Score for player. Goes up after collectings coins, and killing monsters.
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

	* Verison Number added to Config file

### Fixed

	* Monster movement is no longer randomly
	
		* Hopefully to improve suvivabilty between generations


## [1.1.2] - 2015-08-01 - [YANKED]

### Fixed

	* Best Player list no longer appears on show map update

## [1.1.1] - 2015-08-01

### Fixed

	* Increase wait dealy from 20 to 65 so updating is not so jarring.

## [1.1.0] - 2015-07-31

### Added

	* Wait betweent show map: 20 ms
	
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

# Change Log

All notable changes to this project will be documented here.
This project adheres to [Semantic Versioning](http://semver.org/)

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

# Manual for Experimental Platformer AI

This manual will quickly walk you through how to use the program.

## Config File

When you first load the program it will create a config file with the default name "Config.ini" it should look something like the example below.

```

Config File for the program.
v4.0.0
First Generation Steps: 100
Generation Increase: 100
Gens Past Growth: 10
Percent Mutation Chance: 15
Log to File: 1
Hard mode: 0
Show map on update: 0
Random Seed: [12345]
Append Time: 1

```

### Version number

Under the first line you will see a line that has the version of the program that the config file was made for. If you get a new version of the program it will check this number verses its own internal number. 
Since the versioning goes v[Major Revision].[Minor Revision].[Patch Number] the program will interpret the number differences and see if it can use the old config file or not.

A Major Revision change means that it CANNOT use the old config file and a new one will be created. This is because most major revisions that we commit will have a change to the config file. 
Either in a new item, removed item or an item was moved. If the config file is not replaced bad things could happen when the program attempts to load it.

A Minor Revision will cause the program to prompt the user if they want to replace the config or not. Most minor revisions should not change the config and it should be safe to allow it to use the old config file. 
That being said if you notice strange results or the program crashes try replacing the old config file as that could be causing the issue.

A Patch to the program will never affect the config file unless the patch is to the config file. In this cause the program will just load the old config file. If we end up making changes to the config file 
in a patch; then that release will say in it to delete old config files.

### First Generation Steps

The number is the number of steps that the first generation will have. This can be any number from 1 to the Max Player Steps (default is 1000) but a number between 50 and 200 is best.

### Generation Increase

This number is the number increase of steps each generation in the Growth phase. Each generation past the first one will have this many more steps then the generation before it until it hits the Max Player Steps. 
A number between 50 and 200 is best; if you set the First Generation to Max Player Steps then this number should be 0.

### Gens Past Growth

This number is the number of generations that will be taken during the steady phase. During this phase the steps will be equal to the Max Player Steps. 
A number between 0 and 50 is recommended; although the higher the number the "better" the AI should get (in theory).

### Percent Mutation Chance

This is a number between 0 and 99 that is the percent chance that a player will have their direction mutated into something random. This is to give the players a bit more variety between generations 
as a mutation like this could be a really good thing. However if this number is too high then effective all the directions will be random and the AI won't really "learn".

### Log to File

This can be a 1 for true or a 0 for false. If this is true then at the end of a player's "run" all the directions that they took and their fitness will be logged to a file by default 
called "Player.log". This is done to allow a better comparison between the players and let us see how they look.

### Hard Mode

This can be a 1 for true or a 0 for false. When this is enabled the program is put into Hard Mode. Below is a table comparing Normal Mode and Hard Mode.

| Normal Mode                             | Hard Mode                              | 
| --------------------------------------- | -------------------------------------- |
| Fitness will only go up                 | Fitness decreases with number of steps |
| Player can only have a positive fitness | Player with negative fitness will die  |
| Player can go anywhere                  | Player going too far left will die     |

### Show Map

This can be a 1 for true or a 0 for false. If this is enabled then for every step that the player takes the map will be shown. This will of course slow down the program but will allow you to follow 
the player around on their journey.

If this is disabled then you will only see the end result of the Top Players in terms of fitness. It is useful to have log to file on if you disable this so you can see the moves that the player took. 
Disabling it is useful if you want to quickly test if a change has resulted in higher fitness; or are just testing that everything works (this is what we use it for).

### Random Seed

This is a number that can be between 0 and 65,535 that is used to seed rand and allow each run of the program to be slightly different. Read [this] (https://en.wikipedia.org/wiki/Random_seed) if you want to 
know more about seeding. Please ensure that the number is between the brackets in order to be read properly.

This is useful because if you set a specific seed and turn of Append Time (see below) then all the random number generators will always give the same number. This is useful for seeing how change to the map, 
or how players are "mated" affect the fitness. 

### Append Time

This can be a 1 for true or a 0 for false. If this is true then the time will be added to Random Seed to seed all the random number generators. This is used because it helps ensure that each run of the 
program is different because unless the Random Seed & the Time are the EXACT same then all the random numbers should be different (in theory).

This can be disabled as I talked about in Random Seed if don't want the random numbers to be random and instead what the numbers to come out the same.

## Running the Program

Running the program is as simple as double clicking on the .exe to allow it to run. If this is the first time you ran the program then it will create the config file that you can use to change the settings 
that were talked about above.

The program will then go through all of the players through all of the generations on its own with you needing to do anything other than watch.

CC=g++
CFLAGS= -std=c++11 -Os -s
LNFLAGS= -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
source=$(wildcard source/*.cpp)
HEAD=$(wildcard source/*.h)
OBJ=$(source:.cpp=.o)
EXE=Platformer-Experiment

all: $(OBJ) $(HEAD)
	@echo "link $(EXE)"
	@$(CC) $(OBJ) $(CFLAGS) $(LNFLAGS) -o$(EXE)

source/main.o: source/main.cpp source/version.h source/main.h source/map.h source/config.h source/tick.h source/entity.h source/globals.h source/screen.h
source/config.o: source/config.cpp source/config.h source/main.h source/version.h source/globals.h 
source/entity.o: source/entity.cpp source/entity.h source/map.h source/config.h source/globals.h source/main.h
source/map.o: source/map.cpp source/map.h source/config.h source/tick.h source/entity.h source/globals.h 
source/tick.o: source/tick.cpp source/tick.h source/main.h source/globals.h
source/screen.o: source/screen.cpp source/screen.h source/main.h source/globals.h source/config.h source/map.h


clean:
	rm -f source/*.o
	rm -f $(EXE)

.cpp.o:
	@echo "   cc $<"
	@${CC} -c ${CFLAGS} $< -o $@

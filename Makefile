CC=g++
CFLAGS= -std=c++11 -Wall -Wextra -Wno-unused-parameter -g
LNFLAGS=
source=$(wildcard source/*.cpp)
HEAD=$(wildcard source/*.h)
OBJ=$(source:.cpp=.o)
EXE=Platformer-Experiment

all: $(OBJ) $(HEAD)
	@echo "link $(EXE)"
	@$(CC) $(OBJ) $(CFLAGS) $(LNFLAGS) -o$(EXE)

source/main.o: source/main.cpp source/main.h source/map.h source/config.h source/tick.h source/entity.h
source/config.o: source/config.cpp source/config.h source/main.h
source/entity.o: source/entity.cpp source/entity.h source/map.h source/config.h
source/map.o: source/map.cpp source/map.h source/config.h source/tick.h
source/tick.o: source/tick.cpp source/tick.h source/main.h


clean:
	rm -f source/*.o
	rm -f $(EXE)

.cpp.o:
	@echo "   cc $<"
	@${CC} -c ${CFLAGS} $< -o $@

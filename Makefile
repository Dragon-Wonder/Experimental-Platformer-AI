CC=g++
CFLAGS=-c -Wall -g -std=c++11
LDFLAGS=
SOURCES=./Source/main.cpp ./Source/config.cpp ./Source/entity.cpp ./Source/map.cpp ./Source/tick.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Platformer-Experiment

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm edit $(OBJECTS)

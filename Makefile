SHELL:=/bin/bash
CC=g++
CFLAGS=-O0 -g -c -Wall -pthread -Wno-unused-function -std=c++11
LDFLAGS=
SOURCES=Card.cpp Deck.cpp FiveCardDraw.cpp SevenCardStud.cpp TexasHoldEm.cpp Game.cpp Player.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=CardGames

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o $(EXECUTABLE)
	$(RM) $(EXECUTABLE)

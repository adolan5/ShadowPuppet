CC = g++
CXXFLAGS = -std=c++11 -Wall -ggdb
LINKER_FLAGS = -lfreenect -lfreenect_sync -lusb-1.0 -lpthread
EXE = test
SOURCES = K.cc K_main.cc
OBJECTS = $(SOURCES: .cc=.o)

all: $(OBJECTS)
	$(CC) $(CXXFLAGS) -o $(EXE) $^  $(LINKER_FLAGS)

$(OBJECTS): $(SOURCES)
	$(CC) $(CXXFLAGS) $< -c $@


clean:
	-rm -f $(EXE) *.o

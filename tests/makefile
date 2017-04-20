CC = g++
CXXFLAGS = -std=c++11 -Wall -ggdb
LINKER_FLAGS = -lfreenect -lfreenect_sync -lusb-1.0 -lpthread
EXE = test
CXXSRCS = K.cc

all:
	$(CC) $(CXXFLAGS) -o $(EXE) $(CXXSRCS) $(LINKER_FLAGS)

clean:
	-rm -f $(EXE) *.o

CC = g++
C_Flags = -I ./SDL/include -Wall `sdl2-config --cflags`
EXE = test
C_SRCS = test.cc

all:
	$(CC) $(C_Flags) -o $(EXE) $(C_SRCS) `sdl2-config --libs`
	
debug:
	$(CC) $(C_Flags) -ggdb -o $(EXE) $(C_SRCS) `sdl2-config --libs`
	
clean:
	rm -f $(EXE)

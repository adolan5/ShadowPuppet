CC = g++
C_Flags = -I /usr/include/SDL2 -Wall `sdl2-config --cflags`
EXE = test
C_SRCS = test.cc

all: new
	$(CC) $(C_Flags) -o $(EXE) $(C_SRCS) `sdl2-config --libs`
	
new:
	$(CC) $(C_Flags) -o new new.cc

clean:
	rm -f $(EXE) new

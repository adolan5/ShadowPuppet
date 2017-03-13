CC = g++
C_Flags = -L /usr/lib/x86_64-linux-gnu -Wfatal-errors -Wall -lGL -lGLU -lGLEW `sdl2-config --cflags` `sdl2-config --libs`
EXE = test
C_SRCS = test.cc

all:
	$(CC) -o $(EXE) $(C_SRCS) $(C_Flags)
	
debug:
	$(CC) -o $(EXE) $(C_SRCS) -ggdb $(C_Flags)
	
clean:
	rm -f $(EXE)

CC = g++
C_Flags = -Wfatal-errors -Wall `sdl2-config --cflags` `sdl2-config --libs`
LINKER_FLAGS = -lGL -lGLU -lGLEW
EXE = test
C_SRCS = test.cc

all:
	$(CC) -o $(EXE) $(C_SRCS) $(LINKER_FLAGS) $(C_Flags)
	
debug:
	$(CC) -o $(EXE) $(C_SRCS) -ggdb $(LINKER_FLAGS) $(C_Flags)
	
clean:
	rm -f $(EXE)

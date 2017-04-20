CC = g++
C_Flags = -std=c++11 -Wfatal-errors -Wall `sdl2-config --cflags` `sdl2-config --libs`
LINKER_FLAGS = -lGL -lSDL2_image
EXE = test
C_SRCS = test.cc

all:
	$(CC) -o $(EXE) $(C_SRCS) $(LINKER_FLAGS) $(C_Flags)
	
debug:
	$(CC) -o $(EXE) $(C_SRCS) -ggdb $(LINKER_FLAGS) $(C_Flags)
	
clean:
	rm -f $(EXE)

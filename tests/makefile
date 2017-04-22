CC = g++
C_Flags = -std=c++11 -Wfatal-errors -Wall `sdl2-config --cflags` `sdl2-config --libs`
LINKER_FLAGS = -lGL -lSDL2_image
EXE = test
C_SRCS = test.cc

all: render
	$(CC) -o $(EXE) $(C_SRCS) shadowRender.o $(LINKER_FLAGS) $(C_Flags)
	
render: shadowRender.h shadowRender.cc
	$(CC) $(LINKER_FLAGS) $(C_Flags) -c shadowRender.cc
	
debug:
	$(CC) -o $(EXE) $(C_SRCS) -ggdb $(LINKER_FLAGS) $(C_Flags)
	
clean:
	rm -f $(EXE) *.o

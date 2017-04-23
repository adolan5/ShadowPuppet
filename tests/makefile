CC = g++
C_Flags = -std=c++11 -Wfatal-errors -Wall `sdl2-config --cflags` `sdl2-config --libs`
LINKER_FLAGS = -lGL -lSDL2_image
EXE = test
C_SRCS = test.cc

all: render player
	$(CC) -o $(EXE) $(C_SRCS) Player.o shadowRender.o $(LINKER_FLAGS) $(C_Flags)
	
render: shadowRender.h shadowRender.cc
	$(CC) $(LINKER_FLAGS) $(C_Flags) -c shadowRender.cc

debug_render: shadowRender.h shadowRender.cc
	$(CC) $(LINKER_FLAGS) -ggdb $(C_Flags) -c shadowRender.cc
	
player: Player.cc Player.h
	$(CC) $(LINKER_FLAGS) $(C_Flags) -c Player.cc
	
debug: debug_render
	$(CC) -o $(EXE) $(C_SRCS) shadowRender.o -ggdb $(LINKER_FLAGS) $(C_Flags)
	
clean:
	rm -f $(EXE) *.o

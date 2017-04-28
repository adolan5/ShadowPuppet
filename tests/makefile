CC = g++
C_Flags = -std=c++11 -Wfatal-errors -Wall `sdl2-config --cflags` `sdl2-config --libs`
LINKER_FLAGS = -lGL -lSDL2_image
EXE = game

game: shadowRender.o Player.o shadowController.o shadowPuppet.o game.cc
	$(CC) -o $(EXE) game.cc Player.o shadowRender.o shadowController.o shadowPuppet.o $(LINKER_FLAGS) $(C_Flags)
	
shadowRender.o: shadowRender.h shadowRender.cc
	$(CC) $(LINKER_FLAGS) $(C_Flags) -c shadowRender.cc

debug_render: shadowRender.h shadowRender.cc
	$(CC) $(LINKER_FLAGS) -ggdb $(C_Flags) -c shadowRender.cc
	
Player.o: Player.cc Player.h
	$(CC) $(LINKER_FLAGS) $(C_Flags) -c Player.cc
	
debug_player: Player.cc Player.h
	$(CC) $(LINKER_FLAGS) -ggdb $(C_Flags) -c Player.cc
	
shadowController.o: shadowController.h shadowController.cc
	$(CC) $(LINKER_FLAGS) $(C_Flags) -c shadowController.cc
	
debug_controller: shadowController.h shadowController.cc
	$(CC) $(LINKER_FLAGS) -ggdb $(C_Flags) -c shadowController.cc
	
shadowPuppet.o: shadowPuppet.h shadowPuppet.cc
	$(CC) $(LINKER_FLAGS) $(C_Flags) -c shadowPuppet.cc

debug_puppet: shadowPuppet.h shadowPuppet.cc
	$(CC) $(LINKER_FLAGS) -ggdb $(C_Flags) -c shadowPuppet.cc
	
debug: debug_render debug_player debug_controller debug_puppet
	$(CC) -o $(EXE) game.cc Player.o shadowRender.o shadowController.o shadowPuppet.o -ggdb $(LINKER_FLAGS) $(C_Flags)
	
clean:
	rm -f $(EXE) *.o

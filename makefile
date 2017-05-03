CC = g++
CXXFLAGS = -std=c++11 -ggdb `sdl2-config --cflags` `sdl2-config --libs`
LINKER_FLAGS = -lfreenect -lfreenect_sync -lusb-1.0 -lpthread -lGL -lSDL2_image -lSDL2
EXE = Game
SRC_DIR = src
OBJ_DIR = obj
H_DIR = include
SRC = main.cc Render.cc Kinect.cc Controller.cc Player.cc Game.cc
_OBJ = $(SRC:.cc=.o)
OBJ = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))

all: $(EXE)
	@echo Finished.

$(EXE): $(OBJ)
	@echo Linking to EXE.
	@$(CC) $(CXXFLAGS) -o $@ $^ $(LINKER_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@echo Compiling $< to object.
	@$(CC) $(CXXFLAGS) -c $< -o $@ $(LINKER_FLAGS) -I$(H_DIR) 


clean:
	@echo Cleaning files.
	@-rm -f $(EXE) $(OBJ_DIR)/*.o

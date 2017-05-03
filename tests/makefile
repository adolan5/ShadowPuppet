CC = g++
CXXFLAGS = -std=c++11 -ggdb
LINKER_FLAGS = -lfreenect -lfreenect_sync -lusb-1.0 -lpthread
EXE = test
SOURCES = K.cc K_main.cc
OBJECTS = $(SOURCES:.cc=.o)

all: $(EXE)
	@echo Finished.

$(EXE): $(OBJECTS)
	@echo Linking to EXE.
	@$(CC) $(CXXFLAGS) -o $@ $^ $(LINKER_FLAGS)

%.o: %.cc
	@echo Compiling $< to object.
	@$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	@echo Cleaning files.
	@-rm -f $(EXE) *.pbm *.o

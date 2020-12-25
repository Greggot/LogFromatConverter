CC = g++
FLAGS = -Wall
CPP = $(wildcard *.cpp)		#wildcard - find all files via pattern (*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(CPP))	#patsubst - replace one to anoter in a variable
PROGRAM = Log2Arduino

all: $(PROGRAM)
	@echo All Done!

$(PROGRAM) : $(OBJ)
	$(CC) $(FLAGS) -o $(PROGRAM) $^
	@echo "Linking Done..."

$(OBJ) : $(CPP)
	$(CC) $(FLAGS) -c $(CPP)
	@echo "Compilation Done..."

clean: 
	rm -f $(OBJ) $(PROGRAM)
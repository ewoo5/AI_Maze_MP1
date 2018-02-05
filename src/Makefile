IDIR=../include
CC=g++
FLAGS=-I$(IDIR)

ODIR=obj
LDIR=../lib

LIBS=-lm

_OBJ=maze.o solveMaze.o
_DEPS=maze.hpp
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)

solveMaze: $(OBJ)
	$(CC) -o $@ $^ $(FLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

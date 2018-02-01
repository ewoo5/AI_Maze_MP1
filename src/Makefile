IDIR=../include
CC=g++
FLAGS=-I$(IDIR)

ODIR=obj
LDIR=../lib

LIBS=-lm

DEPS=$(patsubst %,$(IDIR)/%.hpp)
OBJ=$(patsubst %,$(ODIR)/%.o)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)

solveMaze.o: $(OBJ)
	$(CC) -o $@ $^ $(FLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

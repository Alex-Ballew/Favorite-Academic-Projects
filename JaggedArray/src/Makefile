CC=gcc
CFLAGS=-I. -g
CPPFLAGS=-I. -g -std=c++11
DEPS =
OBJ = jagged.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	g++ -c -o $@ $< $(CPPFLAGS)

default: $(OBJ)
	$(CC) -o jagged $^ $(CFLAGS)

clean:
	rm -f *.o *.out test/*.o

run: default
	./jagged

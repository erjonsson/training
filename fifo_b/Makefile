CC=g++
CFLAGS=-I.
DEPS = fifo.hpp
OBJ = main.o 
LIBS=-lpthread

%.o: %.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

fifo: $(OBJ)
		g++ -o $@ $^ $(CFLAGS) $(LIBS)

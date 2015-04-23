CC=g++
CFLAGS=-I.
DEPS = fifo_boost.hpp
OBJ = main.o 

%.o: %.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

fifo: $(OBJ)
		g++ -o $@ $^ $(CFLAGS) $(LIBS)

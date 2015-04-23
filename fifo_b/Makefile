CC=g++
CFLAGS=-I -std=c++0x 
DEPS = fifo_boost.hpp
OBJ = main.o 
LIBS = -lpthread


%.o: %.cpp $(DEPS)
		$(CC) -std=c++0x -c -o $@ $< $(CFLAGS)

fifo: $(OBJ)
		g++ -o $@ $^ $(CFLAGS) $(LIBS)

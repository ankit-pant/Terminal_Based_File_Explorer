CC = g++
CFLAGS = -Wall -std=c++14
DEPS = ui.h
OBJ = ui_implement.o main.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
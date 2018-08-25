CC = g++
CFLAGS = -Wall -std=c++14
DEPS = ui.h show_dir.h
OBJ = ui_implement.o show_dir.o main.o 
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
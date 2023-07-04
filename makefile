CC=g++

all: ./obj/main.o ./obj/io_lib.o ./obj/io_lib_prints.o
	$(CC) ./obj/main.o ./obj/io_lib.o ./obj/io_lib_prints.o -o main.exe

./obj/main.o: main.cpp ./include/io_lib.h
	$(CC) -c main.cpp -o ./obj/main.o

./obj/io_lib_prints.o: ./src/io_lib_prints.cpp ./include/io_lib.h
	$(CC) -c ./src/io_lib_prints.cpp -o ./obj/io_lib_prints.o

./obj/io_lib.o: ./src/io_lib.cpp ./include/io_lib.h
	$(CC) -c ./src/io_lib.cpp -o ./obj/io_lib.o

clean:
	-rm -f main.exe obj/*.o

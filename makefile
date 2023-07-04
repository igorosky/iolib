output: ./obj/main.o ./obj/io_lib.o ./obj/io_lib_prints.o
	g++ ./obj/main.o ./obj/io_lib.o ./obj/io_lib_prints.o -o main.exe

./obj/main.o: main.cpp
	g++ -c main.cpp -o ./obj/main.o

./obj/io_lib_prints.o: ./src/io_lib_prints.cpp
	g++ -c ./src/io_lib_prints.cpp -o ./obj/io_lib_prints.o

./obj/io_lib.o: ./src/io_lib.cpp
	g++ -c ./src/io_lib.cpp -o ./obj/io_lib.o

clean:
	-rm -f main.exe obj/*.o

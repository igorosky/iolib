#include <iostream>
#include <windows.h>
#include "include/io_lib.h"
#include <chrono>

uint32_t GetMillis() {
	using namespace std::chrono;
	return static_cast<uint32_t>(duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()).count());
}

using namespace std;

// #include <conio.h>

int main(int argc, char const *argv[]) {
    // while(true) {
    //     if(kbhit()) {
    //         std::cout<<getch()<<'\n';
    //     }
    // }
    // return 0;
    io.AsyncMode();
    io.Println("Hello World", {RED_F});
    for(;;) {
        io.Println("Hello World", {RED_F, GREEN_B});
        auto x = GetMillis() + 2000;
        while(x > GetMillis())
            if(io.InputCount()) io.GetLastInput();
    }
    return 0;
}

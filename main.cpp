#include <iostream>
#include <windows.h>
#include "include/io_lib.h"

using namespace std;

int main(int argc, char const *argv[]) {
    io.AsyncMode();
    io.Println("Hello World", {RED_F});
    for(;;) {
        io.Println("Hello World", {RED_F, GREEN_B});
        Sleep(1000);
        while(io.InputCount()) io.GetLastInput();
    }
    return 0;
}

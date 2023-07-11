# iolib
Library for handling simple async input/output

## Features
- Quckly formated output
- When in async mode
  - Home and End button do thier job
  - Backspace and Delete can erase
  - Insert mode
  - Horizontal arrows for navigation in text
  - Vertical arrows for navigation through history of inputs

## Documentation

### Globals

``IOLib io`` - global object which should be used for everything and no other instance should be created

### Methods

``void AsyncMode(std::string commandPrompt = "> ")`` - turns on async mode (when in async mode io conrolls both input and output and nothing else should take input or print output)

``void DisableAsyncMode(bool waitForProcessEnd = true)`` - goes back to default mode (waitForProcessEnd = true ensure that everything is correctly printed and command prompt is deleted)

``std::size_t InputCount()`` - returns count of inputs ready to fetch

``std::size_t TotalInputCount()`` - returns total count of stored inputs ()

``std::string GetLastInput(bool silent = false)`` - fetch oldest unfetched input. If silent there is no information about it printed. Returns empty string when nothing to fetch

``std::string PeekLastInput()`` - returns std::string which is be next to fetch (without actually fetching it)

``bool isInAsyncMode()`` - returns if io is in async mode

``void setCommandPrompt(const std::string commandPrompt)`` - changes command prompt

``std::string getCommandPrompt()`` - returns current command prompt

``std::string CombineStr(T str, const PrintOptionList args = {})`` - returns string with applied PrintOptions. Macro `p` e.g. ``p("Hello", {RED_F})``

``void EnableOutput()`` - enables output

``void DisableOutput()`` - disables output (when output is disabled what should be printed is just gone)

``bool isOuputEnabled()`` - returns if output is enabled

``std::string toString(T)`` - converts any type to string. It's ment to be used in own specialised templates. When type has specialised template it can be used in CombineStr

``void Print(T str = "", const PrintOptionList args = {})`` - Prints str with applied args (in async mode works same as Println)

``void Println(T str = "", const PrintOptionList args = {})`` - Prints str with applied args and goes to new line

### Atributes

``unsigned short decimalPrecision`` - precision of printed floating points

``std::size_t inputHistorySize`` - count of stored input (history). Max size of std::size_t by default

### Print Options

``RESET`` - sets default everyting back to normal (rather useless because it's done after every single output)
	
``BOLD``
	
``UNDERLINE``
	
``INVERSE`` - changes background color with foreground color
	
``BOLD_OFF``
	
``UNDERLINE_OFF``
	
``INVERSE_OFF``

``<COLOR>_F`` - changes foreground color

``<COLOR>_B`` - changes background color

#### Colors:
- BLACK
- aRED
- GREEN
- YELLOW
- BLUE
- MAGENTA
- CYAN
- WHITE
- GREY
- BRIGHT_RED
- BRIGHT_GREEN
- BRIGHT_YELLOW
- BRIGHT_BLUE
- BRIGHT_MAGENTA
- BRIGHT_CYAN
- BRIGHT_WHITE

### Example
```
#include <iostream>
#include <chrono>   //For Get Millis
#include "include/io_lib.h"

// Used only for creating delay
uint32_t GetMillis() {
	using namespace std::chrono;
	return static_cast<uint32_t>(duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()).count());
}

// Example of custom template for vector<int>
template <>
std::string IOLib::toString(std::vector<int> vec) {
    std::string ans = "[ ";
    std::string comma = "";
    for(const auto& i : vec) {
        ans += comma;
        ans += toString(i);
        comma = ", ";
    }
    ans += " ]";
    return ans;
}

using namespace std;

int main(int argc, char const *argv[]) {
    io.AsyncMode("-> "); // Turn on async mode with "-> " as command prompt
    io.Println(p("Hello", {RED_B, BOLD, UNDERLINE}) + " " + p("World", {GREEN_B, YELLOW_F, BOLD})); // Colorful Hello World
    io.Println(p(1.305)); // Print double rounded to two decimal places
    io.decimalPrecision = 3; // Set decimal precision to three decimal places
    io.Println(p(1.3)); // Print 1.300
    bool loop = true;
    while(loop) {
        // Print Every two seconds and example of that Print beahave the same as Println in async mode
        io.Print("Hello World", {RED_F, GREEN_B});
        io.Println("Have a nice day", {RED_F, GREEN_B});
        auto x = GetMillis() + 2000; // 2s delay
        while(x > GetMillis()) {
            if(!io.InputCount()) continue;
            // exit program on exit
            if(io.PeekLastInput() == "exit") {
                io.Println(io.PeekLastInput() + " :-)");
                (void)io.GetLastInput(true); // Fetch in silence so there will be nothing left after pressing enter
                return 0;
            }
            string str = io.GetLastInput();
            if(str == "stop") {
                loop = false;
                break;
            }
            // Change command prompt
            else if(str == "loonger") {
                (void)io.GetLastInput(false);
                io.setCommandPrompt("-" + io.getCommandPrompt());
            }
            else
                io.Println("Unknown command", {BOLD, RED_F});
        }
    }
    io.DisableAsyncMode();
    io.Print("Good bye");
    io.Print('!'); // Print works as it should in non-async mode
    io.Println(); // Go to new line
    vector<int> coolVector = {1,2,3,4};
    io.Println(p(coolVector)); // Use of custom template for vectors
    return 0;
}
```

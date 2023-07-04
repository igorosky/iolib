// Author: Igor Zaworski
#include <iostream>
#include "../include/io_lib.h"


void IOLib::_print(const std::string& str, const PrintOptionList &args) {
    _output.push_back({
        str: CombineStr(str, args),
    });
}

void IOLib::_print(const std::string& str, const unsigned short count, va_list args) {
    PrintOptionList params;
    for(int i = 0; i < count; i++) 
        params.push_back((PrintOptions)va_arg(args, int));
    _print(str, params);
}

void IOLib::Println() {
    _print("\r\n", {});
}



void IOLib::Println(const std::string str, const unsigned short count, ...) {
    va_list args;
    va_start(args, count);
    _print(str, count, args);
    va_end(args);
    
}

void IOLib::Println(const std::string str, const PrintOptionList args) {
    _print(str, args);
    
}



void IOLib::Println(const char str, const unsigned short count, ...) {
    va_list args;
    va_start(args, count);
    const std::string c = std::string(1, str);
    _print(c, count, args);
    va_end(args);
    
}

void IOLib::Println(const char str, const PrintOptionList args) {
    _print(std::string(1, str), args);
    
}



void IOLib::Println(const int str, const unsigned short count, ...) {
    va_list args;
    va_start(args, count);
    const std::string c = std::to_string(str);
    _print(c, count, args);
    va_end(args);
    
}

void IOLib::Println(const int str, const PrintOptionList args) {
    _print(std::to_string(str), args);
    
}

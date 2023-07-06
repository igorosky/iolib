// Author: Igor Zaworski
#include <iostream>
#include "../include/io_lib.h"

template<>
std::string IOLib::toString(const std::string str) {
    return str;
}

template<>
std::string IOLib::toString(const char* str) {
    return std::string(str);
}

template<>
std::string IOLib::toString(const double str) {
    return std::to_string(str);
}

template<>
std::string IOLib::toString(const float str) {
    return std::to_string(str);
}

template<>
std::string IOLib::toString(const unsigned char str) {
    return std::string(1, str);
}

template<>
std::string IOLib::toString(const char str) {
    return std::string(1, str);
}

template<>
std::string IOLib::toString(const unsigned short str) {
    return std::to_string(str);
}

template<>
std::string IOLib::toString(const short str) {
    return std::to_string(str);
}

template<>
std::string IOLib::toString(const unsigned int str) {
    return std::to_string(str);
}

template<>
std::string IOLib::toString(const int str) {
    return std::to_string(str);
}

template<>
std::string IOLib::toString(const unsigned long str) {
    return std::to_string(str);
}

template<>
std::string IOLib::toString(const long str) {
    return std::to_string(str);
}

template<>
std::string IOLib::toString(const unsigned long long str) {
    return std::to_string(str);
}

template<>
std::string IOLib::toString(const long long str) {
    return std::to_string(str);
}

template<>
std::string IOLib::toString(const bool str) {
    return str ? "true" : "false";
}

// template <typename Y, typename U>
// std::string IOLib::toString<Y, U, std::pair<Y, U>>(std::pair<Y, U> pair) {
//     return "(" + toString(pair.first) + ", " + totring(pair.second) + ")";
// }

// template <typename Y, typename U>
// std::string IOLib::toString<Y, U, std::vector<Y, U>>(std::vector<Y, U> vec) {
//     std::string ans = "[ ";
//     std::string comma = "";
//     for(const auto& i : vec) {
//         ans += comma;
//         ans += toString(i);
//         comma = ", ";
//     }
//     ans += " ]"
//     return ans;
// }

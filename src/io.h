#pragma once

#include <fstream>
#include <vector>
#include <string>

std::ifstream OpenFileRead(const std::string& path);
std::ofstream OpenFileWrite(const std::string& path);

template<typename T>
void Read(std::istream& in, T* t, size_t n) {
    auto p = reinterpret_cast<char*>(t);
    in.read(p, n * sizeof(T));
}

template<typename T>
T Read(std::istream& in) {
    T result;
    Read(in, &result, 1);

    return result;
}

template<typename T>
std::vector<T> ReadVec(std::istream& in, size_t n) {
    std::vector<T> result(n);
    Read(in, result.data(), n);

    return result;
}

template<typename T>
void Write(std::ostream& out, const T* t, size_t n) {
    auto p = reinterpret_cast<const char*>(t);
    out.write(p, n * sizeof(T));
}

template<typename T>
void Write(std::ostream& out, const T& t) {
    Write(out, &t, 1);
}

template<typename T>
void WriteVec(std::ostream& out, const std::vector<T>& vec) {
    Write(out, vec.data(), vec.size());
}

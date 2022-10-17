#include "io.h"

std::ifstream OpenFileRead(const std::string& path) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Couldn't open '" + path + "' for reading");

    return file;
}

std::ofstream OpenFileWrite(const std::string& path) {
    std::ofstream file(path);
    if (!file) throw std::runtime_error("Couldn't open '" + path + "' for writing");

    return file;
}

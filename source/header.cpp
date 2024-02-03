#include <fstream>
#include <cstring>
#include "header.hpp"

void readHeaderFromFile(const std::string &file, retron5Header &headerOut)
{
    // Clear the header before reading into it.
    std::memset(&headerOut, 0x00, sizeof(retron5Header));

    // Open the file and read the header
    std::ifstream saveFile(file, std::ios::binary);
    saveFile.read(reinterpret_cast<char *>(&headerOut), sizeof(retron5Header));
}
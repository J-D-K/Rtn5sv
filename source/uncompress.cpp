#include <iostream>
#include <fstream>
#include <zlib.h>
#include "uncompress.hpp"

void uncompressSave(const retron5Header &header, const std::string &saveIn, const std::string &saveOut)
{
    std::cout << "Decompressing \"" << saveIn << "\" to \"" << saveOut << "\"... ";
 
    // I/O Files
    std::ifstream fileIn(saveIn, std::ios::binary);
    std::ofstream fileOut(saveOut, std::ios::binary);

    if(!fileIn.is_open())
    {
        std::cout << "Error opening file for reading!\n";
        return;
    }
    else if(!fileOut.is_open())
    {
        std::cout << "Error opening file for writing!\n";
        return;
    }

    // Buffers to read and decompress to since zlib is C
    Bytef *readBuffer = new Bytef[header.compressedSize];
    Bytef *uncompressBuffer = new Bytef[header.uncompressedSize];

    // Seek to after offset and read the actual save data
    fileIn.seekg(header.offset, std::ios::beg);
    fileIn.read(reinterpret_cast<char *>(readBuffer), header.compressedSize);

    // Uncompress data with zlib : To do: error check
    int uncompressError = uncompress(uncompressBuffer, reinterpret_cast<uLongf *>(header.uncompressedSize), readBuffer, header.compressedSize);

    // Needed to make sure save will work on system
    uint32_t saveCRC32 = crc32(0, uncompressBuffer, header.uncompressedSize);

    if(saveCRC32 == header.crc32)
    {
        fileOut.write(reinterpret_cast<char *>(uncompressBuffer), header.uncompressedSize);
        std::cout << "Done!\n";
    }
    else
    {
        std::cout << "CRC32 does not match source!\n";
    }

    // Free buffers
    delete[] readBuffer;
    delete[] uncompressBuffer;
}
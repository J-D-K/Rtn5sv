#include <iostream>
#include <fstream>
#include <zlib.h>
#include "compress.hpp"
#include "header.hpp"

void compressSave(const std::string &saveIn, const std::string &saveOut)
{
    std::cout << "Compressing \"" << saveIn << "\" to \"" << saveOut << "\"... ";

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

    // Make new retron header
    retron5Header header;
    header.magic = RETRON_5_HEADER_MAGIC;
    header.version = 1;
    header.flags = 1;
    header.offset = sizeof(retron5Header);

    // Get file size for header and zlib compress
    fileIn.seekg(std::ios::end);
    header.uncompressedSize = fileIn.tellg();
    header.compressedSize = header.uncompressedSize;

    // Buffers
    Bytef *readBuffer = new Bytef[header.uncompressedSize];
    Bytef *compressionBuffer = new Bytef[header.uncompressedSize];

    // Read and get CRC32
    fileIn.read(reinterpret_cast<char *>(readBuffer), header.uncompressedSize);
    header.crc32 = crc32(0, readBuffer, header.uncompressedSize);

    // Zlib compress
    compress(compressionBuffer, reinterpret_cast<uLongf *>(&header.compressedSize), readBuffer, header.uncompressedSize);

    // Write header
    fileOut.write(reinterpret_cast<char *>(&header), sizeof(retron5Header));

    // Write data
    fileOut.write(reinterpret_cast<char *>(compressionBuffer), header.compressedSize);

    delete[] readBuffer;
    delete[] compressionBuffer;
}
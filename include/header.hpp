#pragma once
#include <string>
#include <cstdint>

// This is what the magic should be
#define RETRON_5_HEADER_MAGIC 0x354E5452

// This is the header struct in the beginning of the saves
typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint16_t flags;
    uint32_t uncompressedSize;
    uint32_t compressedSize;
    uint32_t offset;
    uint32_t crc32;
} retron5Header;

// Reads header from 'file' into 'headerOut'
void readHeaderFromFile(const std::string &file, retron5Header &headerOut);
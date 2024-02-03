#pragma once
#include <string>
#include "header.hpp"

// Uses zlib uncompress to create a PC emulator readable save/*.srm
void uncompressSave(const retron5Header &header, const std::string &saveIn, const std::string &saveOut);
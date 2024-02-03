#pragma once
#include <string>

// Compresses and creates and Retron5 compatible save from (most) emulator saves
void compressSave(const std::string& saveIn, const std::string &saveOut);
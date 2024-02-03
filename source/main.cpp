#include <iostream>
#include <string>
#include <zlib.h>
#include "header.hpp"
#include "uncompress.hpp"
#include "compress.hpp"

//rnt5sv - tool to convert to and from retron5 saves by JK Written in C++

//Replaces the file extension in path with newExtension
std::string getReplacedExtension(const std::string &path, const std::string &newExtension)
{
    std::string returnString = path;

    // Find where the extension is
    size_t extensionPosition = returnString.find_last_of('.');

    // If it doesn't have an extension, just append it. Replace if it does
    if(extensionPosition == returnString.npos)
    {
        returnString.append(newExtension);
    }
    else
    {
        returnString.replace(extensionPosition, returnString.npos - extensionPosition, newExtension);
    }
    return returnString;
}

int main(int argc, const char *argv[])
{
    if(argc < 1)
    {
        std::cout << "Usage: rtn5sv [files]\n";
        return -1;
    }

    std::cout << "rtn5sv\nOpen source Retron5 Save converter written in C++ (because boredom) - JK\n";
    // Header to use for testing files
    retron5Header header;
    for(int i = 1; i < argc; i++)
    {
        readHeaderFromFile(argv[i], header);
        if(header.magic == RETRON_5_HEADER_MAGIC)
        {   
            std::string fileOut = getReplacedExtension(argv[i], "srm");
            uncompressSave(header, argv[i], fileOut);
        }
        else
        {
            std::string fileOut = getReplacedExtension(argv[i], "sav");
            compressSave(argv[i], fileOut);
        }
    }
    return 0;
}

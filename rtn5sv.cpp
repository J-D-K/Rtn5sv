#include <cstdio>
#include <cstring>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <zlib.h>

//rnt5sv - tool to convert to and from retron5 saves by JK

typedef struct
{
    uint32_t mag;
    uint16_t ver;
    uint16_t flags;
    uint32_t orignalSize;
    uint32_t compressedSize;
    uint32_t offset;
    uint32_t crc;
} rtn5Head;

rtn5Head readHeader(const std::string& file)
{
    rtn5Head ret = {0};
    FILE *f = fopen(file.c_str(), "rb");
    fread(&ret, sizeof(rtn5Head), 1, f);
    fclose(f);
    return ret;
}

std::string replaceExt(const std::string& path, const std::string& newExt)
{
    std::string ret = path;
    size_t extPos = ret.find_last_of('.') + 1;
    if(extPos == ret.npos)
        ret += newExt;
    else
        ret.replace(extPos, newExt.length(), newExt);
    return ret;
}

void decompress(const rtn5Head& head, const std::string& file)
{
    //Files to read/write
    FILE *saveIn, *srmOut;
    //Buffer to read into and buffer to decompress to
    uint8_t *readBuff = new uint8_t[head.compressedSize];
    uint8_t *decompBuff = new uint8_t[head.orignalSize];

    //Open, seek to offset (after header) and read
    saveIn = fopen(file.c_str(), "rb");
    fseek(saveIn, head.offset, SEEK_SET);
    fread(readBuff, 1, head.compressedSize, saveIn);
    fclose(saveIn);

    //Use zlib uncompress.
    uncompress(decompBuff, (uLongf *)&head.orignalSize, readBuff, head.compressedSize);

    //Check CRC JIC
    uint32_t crc = crc32(0, decompBuff, head.orignalSize);
    if(crc == head.crc)
    {
        std::string out = replaceExt(file, "srm");
        srmOut = fopen(out.c_str(), "wb");
        fwrite(decompBuff, 1, head.orignalSize, srmOut);
        fclose(srmOut);
    }
    else
        printf("CRC32 does not match.\n");

    delete[] readBuff;
    delete[] decompBuff;
}

void rtn5Compress(const std::string& file)
{
    //New header
    rtn5Head head;
    head.mag = 0x354E5452;
    head.ver = 1;
    head.flags = 1;
    head.offset = sizeof(rtn5Head);

    //Get file size
    FILE *saveIn = fopen(file.c_str(), "rb");
    fseek(saveIn, 0, SEEK_END);
    head.orignalSize = ftell(saveIn);
    fseek(saveIn, 0, SEEK_SET);
    head.compressedSize = head.orignalSize;

    //Read/compress buffers
    uint8_t *readBuff = new uint8_t[head.orignalSize];
    uint8_t *compBuff = new uint8_t[head.orignalSize];

    //Read file + get CRC32 for header
    fread(readBuff, 1, head.orignalSize, saveIn);
    head.crc = crc32(0, readBuff, head.orignalSize);

    //Run zlib compress
    compress(compBuff, (uLongf *)&head.compressedSize, readBuff, head.orignalSize);

    //Open output file, write header + data
    std::string out = replaceExt(file, "sav");
    FILE *saveOut = fopen(out.c_str(), "wb");
    fwrite(&head, sizeof(rtn5Head), 1, saveOut);
    fwrite(compBuff, 1, head.compressedSize, saveOut);

    //Cleanup
    fclose(saveIn);
    fclose(saveOut);
    delete[] readBuff;
    delete[] compBuff;
}

int main(int argc, const char *argv[])
{
    if(argc < 1)
    {
        printf("rtn5sv [files]\n");
        return -1;
    }

    printf("rtn5sv - JK\n");
    for(int i = 1; i < argc; i++)
    {
        rtn5Head head = readHeader(argv[i]);
        if(head.mag == 0x354E5452)
            decompress(head, argv[i]);
        else
            rtn5Compress(argv[i]);
    }
}

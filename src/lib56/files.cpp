#include <cstdio>
#include <cstdlib>

char* readFile(const char* filePath, int* length) {
    FILE* file = fopen(filePath, "rb");
    if(!file)
        return nullptr;
    fseek(file, 0, SEEK_END);
    *length = (int)ftell(file);
    char* tmpBuffer = (char*) malloc(*length);
    if(!tmpBuffer)
        return nullptr;
    rewind(file);
    fread(tmpBuffer, *length, 1, file);
    fclose(file);
    return tmpBuffer;
}

int writeFile(char* filePath, int length, void* buffer) {
    FILE* file = fopen(filePath, "wb");
    if(!file)
        return 1;
    int tmpValue = (int) fwrite(buffer, 1, length, file);
    fclose(file);
    return tmpValue != length;
}
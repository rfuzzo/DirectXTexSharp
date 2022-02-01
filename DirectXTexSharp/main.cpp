//
// Created by moritz.baron on 31/01/2022.
//

#include <iostream>
#include <fstream>
#include "texconv.h"

void error(const char *s, const char *curfile = NULL) {
    if (curfile)
        fprintf(stderr, "%s: ", curfile);
    fprintf(stderr, "%s\n", s);
    exit(1);
}

byte *load_file(const char *filename, int *size) {
    FILE *f = fopen(filename, "rb");
    if (!f) error("file open error", filename);
    fseek(f, 0, SEEK_END);
    int packed_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    byte *input = new byte[packed_size];
    if (!input) error("memory error", filename);
    if (fread(input, 1, packed_size, f) != packed_size) error("error reading", filename);
    fclose(f);
    *size = packed_size;
    return input;
}

int main() {
    const char* curfile = R"(E:\GitHub\DirectXTexSharp\DirectXTexSharp\test.tga)";

    int input_size;
    byte *input = load_file(curfile, &input_size);

    Blob blob;
    auto hr = ConvertToDds(input, input_size, blob,
                           DirectXTexSharp::ESaveFileTypes::TGA,DXGI_FORMAT_UNKNOWN,
                           false, false);
    if (hr > 0)
    {
        return 1;
    }

    auto len = blob.m_size;
    //auto buffer = blob.GetBufferPointer();
    auto buffer =  static_cast<byte*>(blob.m_buffer);

    const char *destfile = R"(E:\GitHub\DirectXTexSharp\DirectXTexSharp\test_out.dds)";
    FILE *f = fopen(destfile, "wb");
    if (!f) error("file open for write error", destfile);
    if (fwrite(buffer, 1, len, f) != len)
        error("file write error", destfile);
    fclose(f);

    return 0;
}
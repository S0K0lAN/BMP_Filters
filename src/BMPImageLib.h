#pragma once
#include "pack_defines.h"
#include <vector>
#include <iostream>
#include <cstdint>

struct RGB {
    float r, g, b, a;
};

PACKED_STRUCT_BEGIN BitmapFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} PACKED_STRUCT_END

class BMPImage {
private:
    std::vector<std::vector<RGB>> pixels_;
    uint32_t width_;
    uint32_t height_;
    BitmapFileHeader bitmapfileheader_;
    BitmapInfoHeader bitmapinfoheader_;
public:
    bool read(std::istream&);
    void write(std::ostream&);
    RGB GetPixel(int x, int y);
    void SetPixel(int x, int y, RGB);
    int GetWidth();
    int GetHeight();
    
    void SetPixels(const std::vector<std::vector<RGB>>& pixels);
};
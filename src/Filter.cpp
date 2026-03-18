#include "Filter.h"
#include <algorithm>
#include <cstdint>
#include <iostream>

void GrayscaleFilter::Apply(BMPImage& image) {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            RGB pixel = image.GetPixel(x, y);
            
            int r = static_cast<int>(pixel.r);
            int g = static_cast<int>(pixel.g);
            int b = static_cast<int>(pixel.b);
            
            int gray = static_cast<int>(0.299f * r + 0.587f * g + 0.114f * b);
            gray = std::clamp(gray, 0, 255);
            
            pixel.r = static_cast<std::byte>(gray);
            pixel.g = static_cast<std::byte>(gray);
            pixel.b = static_cast<std::byte>(gray);
            
            image.SetPixel(x, y, pixel);
        }
    }
}
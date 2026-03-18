#include "BMPImageLib.h"

static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

bool BMPImage::read(std::istream& ifs) {
    if(!ifs) {
        return false;
    }

    ifs.read(reinterpret_cast<char*>(&bitmapfileheader_), sizeof(bitmapfileheader_));
    ifs.read(reinterpret_cast<char*>(&bitmapinfoheader_), sizeof(bitmapinfoheader_));

    if (bitmapinfoheader_.biPlanes != 1 || bitmapinfoheader_.biBitCount != 24 || bitmapinfoheader_.biCompression != 0) {
        return false;
    }

    const int width = bitmapinfoheader_.biWidth;
    const int height = bitmapinfoheader_.biHeight;
    if (width <= 0 || height <= 0) {
        return false;
    }

    const int stride = GetBMPStride(width);
    pixels_ = std::vector<std::vector<RGB>>(height, std::vector<RGB>(width));
    std::vector<char> buffer(stride);

    for(int y = height - 1; y >= 0; --y) {
        ifs.read(buffer.data(), stride);
        if(!ifs) {
            return false;
        }
        
        for(int x = 0; x < width; ++x) {
            int idx = x*3;
            pixels_[y][x].b = static_cast<std::byte>(buffer[idx]);
            pixels_[y][x].g = static_cast<std::byte>(buffer[idx + 1]);
            pixels_[y][x].r = static_cast<std::byte>(buffer[idx + 2]);
            pixels_[y][x].a = static_cast<std::byte>(255);
        }
    }
    
    return true;
}

void BMPImage::write(std::ostream& ofs) {
    const int width = GetWidth();
    const int height = GetHeight();
    const int stride = GetBMPStride(width);
    
    ofs.write(reinterpret_cast<char*>(&bitmapfileheader_), sizeof(bitmapfileheader_));
    ofs.write(reinterpret_cast<char*>(&bitmapinfoheader_), sizeof(bitmapinfoheader_));
    
    std::vector<char> buffer(stride, 0);
    
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            int idx = x * 3;
            buffer[idx] = static_cast<char>(pixels_[y][x].b);
            buffer[idx + 1] = static_cast<char>(pixels_[y][x].g);
            buffer[idx + 2] = static_cast<char>(pixels_[y][x].r);
        }
        ofs.write(buffer.data(), stride);
    }
}

RGB BMPImage::GetPixel(int x, int y) {
    return pixels_[y][x];
}

void BMPImage::SetPixel(int x, int y, RGB pixel) {
    pixels_[y][x] = pixel;
}

int BMPImage::GetWidth() {
    return bitmapinfoheader_.biWidth;
}

int BMPImage::GetHeight() {
    return bitmapinfoheader_.biHeight;
}
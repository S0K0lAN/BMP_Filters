#include "BMPImageLib.h"

static int GetBMPStride(int w)
{
    return 4 * ((w * 3 + 3) / 4);
}

bool BMPImage::read(std::istream &ifs)
{
    if (!ifs)
    {
        return false;
    }

    ifs.read(reinterpret_cast<char *>(&bitmapfileheader_), sizeof(bitmapfileheader_));
    ifs.read(reinterpret_cast<char *>(&bitmapinfoheader_), sizeof(bitmapinfoheader_));

    if (bitmapinfoheader_.biPlanes != 1 || bitmapinfoheader_.biBitCount != 24 || bitmapinfoheader_.biCompression != 0)
    {
        return false;
    }

    const int width = bitmapinfoheader_.biWidth;
    const int height = bitmapinfoheader_.biHeight;
    if (width <= 0 || height <= 0)
    {
        return false;
    }

    const int stride = GetBMPStride(width);
    pixels_ = std::vector<std::vector<RGB>>(height, std::vector<RGB>(width));
    std::vector<char> buffer(stride);

    for (int y = height - 1; y >= 0; --y)
    {
        ifs.read(buffer.data(), stride);
        if (!ifs)
        {
            return false;
        }

        for (int x = 0; x < width; ++x)
        {
            int idx = x * 3;
            pixels_[y][x].b = static_cast<unsigned char>(buffer[idx]) / 255.0f;
            pixels_[y][x].g = static_cast<unsigned char>(buffer[idx + 1]) / 255.0f;
            pixels_[y][x].r = static_cast<unsigned char>(buffer[idx + 2]) / 255.0f;
            pixels_[y][x].a = 1.0f;
        }
    }

    return true;
}

void BMPImage::write(std::ostream &ofs)
{
    const int width = GetWidth();
    const int height = GetHeight();
    const int stride = GetBMPStride(width);

    bitmapinfoheader_.biWidth = width;
    bitmapinfoheader_.biHeight = height;
    bitmapinfoheader_.biSizeImage = stride * height;
    bitmapfileheader_.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + bitmapinfoheader_.biSizeImage;

    ofs.write(reinterpret_cast<char *>(&bitmapfileheader_), sizeof(bitmapfileheader_));
    ofs.write(reinterpret_cast<char *>(&bitmapinfoheader_), sizeof(bitmapinfoheader_));

    std::vector<char> buffer(stride, 0);

    for (int y = height - 1; y >= 0; --y)
    {
        for (int x = 0; x < width; ++x)
        {
            int idx = x * 3;
            buffer[idx] = static_cast<char>(static_cast<int>(pixels_[y][x].b * 255.0f));
            buffer[idx + 1] = static_cast<char>(static_cast<int>(pixels_[y][x].g * 255.0f));
            buffer[idx + 2] = static_cast<char>(static_cast<int>(pixels_[y][x].r * 255.0f));
        }
        ofs.write(buffer.data(), stride);
    }
}

RGB BMPImage::GetPixel(int x, int y)
{
    return pixels_[y][x];
}

void BMPImage::SetPixel(int x, int y, RGB pixel)
{
    pixels_[y][x] = pixel;
}

int BMPImage::GetWidth()
{
    return bitmapinfoheader_.biWidth;
}

int BMPImage::GetHeight()
{
    return bitmapinfoheader_.biHeight;
}

void BMPImage::SetPixels(const std::vector<std::vector<RGB>> &pixels)
{
    if (pixels.empty() || pixels[0].empty())
    {
        return;
    }

    int newHeight = static_cast<int>(pixels.size());
    int newWidth = static_cast<int>(pixels[0].size());
    pixels_ = pixels;

    bitmapinfoheader_.biWidth = newWidth;
    bitmapinfoheader_.biHeight = newHeight;

    int stride = GetBMPStride(newWidth);
    bitmapinfoheader_.biSizeImage = stride * newHeight;

    bitmapfileheader_.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + bitmapinfoheader_.biSizeImage;
}
#include "Filter.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <cmath>

static RGB GetPixelClamped(BMPImage& image, int x, int y) {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    x = std::clamp(x, 0, width - 1);
    y = std::clamp(y, 0, height - 1);
    
    return image.GetPixel(x, y);
}

void CropFilter::Apply(BMPImage& image) {
    int originalWidth = image.GetWidth();
    int originalHeight = image.GetHeight();
    
    int newWidth = std::min(width_, originalWidth);
    int newHeight = std::min(height_, originalHeight);
    
    std::vector<std::vector<RGB>> newPixels(newHeight, std::vector<RGB>(newWidth));
    
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            newPixels[y][x] = image.GetPixel(x, y);
        }
    }
    image.SetPixels(std::move(newPixels));
}

void NegativeFilter::Apply(BMPImage& image) {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            RGB pixel = image.GetPixel(x, y);
            
            pixel.r = 1.0 - pixel.r;
            pixel.g = 1.0 - pixel.g;
            pixel.b = 1.0 - pixel.b;
            
            image.SetPixel(x, y, pixel);
        }
    }
}

void GrayscaleFilter::Apply(BMPImage& image) {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            RGB pixel = image.GetPixel(x, y);
            
            double gray = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;
            
            pixel.r = gray;
            pixel.g = gray;
            pixel.b = gray;
            
            image.SetPixel(x, y, pixel);
        }
    }
}

void EdgeDetectionFilter::Apply(BMPImage& image) {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    std::vector<std::vector<double>> grayImage(height, std::vector<double>(width));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            RGB pixel = image.GetPixel(x, y);
            grayImage[y][x] = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;
        }
    }
    std::vector<std::vector<double>> matrix = {
        {0, -1, 0},
        {-1, 4, -1},
        {0, -1, 0}
    };
    
    std::vector<std::vector<double>> result(height, std::vector<double>(width));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double sum = 0.0;
            
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int nx = x + kx;
                    int ny = y + ky;
                    
                    nx = std::clamp(nx, 0, width - 1);
                    ny = std::clamp(ny, 0, height - 1);
                    
                    sum += grayImage[ny][nx] * matrix[ky + 1][kx + 1];
                }
            }
            
            double normalized = std::clamp((sum + 4.0) / 8.0, 0.0, 1.0);
            result[y][x] = normalized;
        }
    }
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            RGB newPixel;
            
            if (result[y][x] > threshold_) {
                newPixel.r = 1.0;
                newPixel.g = 1.0;
                newPixel.b = 1.0;
            } else {
                newPixel.r = 0.0;
                newPixel.g = 0.0;
                newPixel.b = 0.0;
            }
            newPixel.a = 1.0;
            
            image.SetPixel(x, y, newPixel);
        }
    }
}

void MatrixFilter::Apply(BMPImage& image) {
    int width = image.GetWidth();
    int height = image.GetHeight();
    int matrixSize = matrix_.size();
    int offset = matrixSize / 2;
    
    std::vector<std::vector<RGB>> original(height, std::vector<RGB>(width));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            original[y][x] = image.GetPixel(x, y);
        }
    }
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            
            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    int nx = x + kx;
                    int ny = y + ky;
                    
                    nx = std::clamp(nx, 0, width - 1);
                    ny = std::clamp(ny, 0, height - 1);
                    
                    RGB pixel = original[ny][nx];
                    double weight = matrix_[ky + offset][kx + offset];
                    
                    sumR += pixel.r * weight;
                    sumG += pixel.g * weight;
                    sumB += pixel.b * weight;
                }
            }
            
            RGB newPixel;
            newPixel.r = std::clamp(sumR, 0.0, 1.0);
            newPixel.g = std::clamp(sumG, 0.0, 1.0);
            newPixel.b = std::clamp(sumB, 0.0, 1.0);
            newPixel.a = 1.0;
            
            image.SetPixel(x, y, newPixel);
        }
    }
}

SharpeningFilter::SharpeningFilter() 
    : MatrixFilter({{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}}) {
}

void SharpeningFilter::Apply(BMPImage& image) {
    MatrixFilter::Apply(image);
}

std::vector<double> GaussianFilter::GenerateGaussianKernel(double sigma) {
    int size = static_cast<int>(std::ceil(sigma * 3)) * 2 + 1;
    if (size < 3) size = 3;
    
    std::vector<double> kernel(size);
    double sum = 0.0;
    int center = size / 2;
    
    for (int i = 0; i < size; ++i) {
        int dx = i - center;
        kernel[i] = std::exp(-(dx*dx) / (2 * sigma * sigma));
        sum += kernel[i];
    }
    for (int i = 0; i < size; ++i) {
        kernel[i] /= sum;
    }
    
    return kernel;
}

GaussianFilter::GaussianFilter(double sigma) 
    : sigma_(sigma) {
}

void GaussianFilter::Apply(BMPImage& image) {
    auto kernel = GenerateGaussianKernel(sigma_);
    int width = image.GetWidth();
    int height = image.GetHeight();
    int ksize = kernel.size();
    int offset = ksize / 2;
    
    // Horizontal blur
    std::vector<std::vector<RGB>> temp(height, std::vector<RGB>(width));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for (int k = -offset; k <= offset; ++k) {
                int nx = std::clamp(x + k, 0, width - 1);
                RGB p = image.GetPixel(nx, y);
                double w = kernel[k + offset];
                sumR += p.r * w;
                sumG += p.g * w;
                sumB += p.b * w;
            }
            temp[y][x] = {static_cast<float>(std::clamp(sumR, 0.0, 1.0)), static_cast<float>(std::clamp(sumG, 0.0, 1.0)), static_cast<float>(std::clamp(sumB, 0.0, 1.0)), 1.0f};
        }
    }
    
    // Vertical blur
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for (int k = -offset; k <= offset; ++k) {
                int ny = std::clamp(y + k, 0, height - 1);
                RGB p = temp[ny][x];
                double w = kernel[k + offset];
                sumR += p.r * w;
                sumG += p.g * w;
                sumB += p.b * w;
            }
            image.SetPixel(x, y, {static_cast<float>(std::clamp(sumR, 0.0, 1.0)), static_cast<float>(std::clamp(sumG, 0.0, 1.0)), static_cast<float>(std::clamp(sumB, 0.0, 1.0)), 1.0f});
        }
    }
}
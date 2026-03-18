#pragma once
#include <string>
#include <vector>
#include "BMPImageLib.h"

struct FilterDescriptor {
    std::string name;
    std::vector<std::string> parameters;
};

class IFilter {
public:
    virtual ~IFilter() = default;
    virtual void Apply(BMPImage& image) = 0;
};

class CropFilter : public IFilter {
public:
    void Apply(BMPImage& image) override;
};

class NegativeFilter : public IFilter {
public:
    void Apply(BMPImage& image) override;
};

class GrayscaleFilter : public IFilter {
public:
    void Apply(BMPImage& image) override;
};

class MatrixFilter : public IFilter {
public:
    void Apply(BMPImage& image) override = 0;
};

class SharpeningFilter : public MatrixFilter {
public:
    void Apply(BMPImage& image) override;
};

class GaussianFilter : public MatrixFilter {
public:
    void Apply(BMPImage& image) override;
};

class EdgeDetectionFilter : public MatrixFilter {
public:
    void Apply(BMPImage& image) override;
};
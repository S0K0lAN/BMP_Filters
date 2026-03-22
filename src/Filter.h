#pragma once
#include <string>
#include <vector>
#include "BMPImageLib.h"

struct FilterDescriptor
{
    std::string name;
    std::vector<std::string> parameters;
};

class IFilter
{
public:
    virtual ~IFilter() = default;
    virtual void Apply(BMPImage &image) = 0;
};

class CropFilter : public IFilter
{
private:
    int width_;
    int height_;

public:
    CropFilter(int width = 800, int height = 600) : width_(width), height_(height) {}
    void Apply(BMPImage &image) override;
};

class NegativeFilter : public IFilter
{
public:
    void Apply(BMPImage &image) override;
};

class GrayscaleFilter : public IFilter
{
public:
    void Apply(BMPImage &image) override;
};

class EdgeDetectionFilter : public IFilter
{
private:
    double threshold_;

public:
    EdgeDetectionFilter(double threshold = 0.1) : threshold_(threshold) {}
    void Apply(BMPImage &image) override;
};

class MatrixFilter : public IFilter
{
protected:
    std::vector<std::vector<double>> matrix_;

public:
    MatrixFilter(const std::vector<std::vector<double>> &matrix) : matrix_(matrix) {}
    void Apply(BMPImage &image) override;
};

class SharpeningFilter : public MatrixFilter
{
public:
    SharpeningFilter();
    void Apply(BMPImage &image) override;
};

class GaussianFilter : public MatrixFilter
{
private:
    double sigma_;
    static std::vector<std::vector<double>> GenerateGaussianMatrix(double sigma);

public:
    GaussianFilter(double sigma = 1.0);
    void Apply(BMPImage &image) override;
};
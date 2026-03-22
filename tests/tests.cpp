#include <iostream>
#include <fstream>
#include <filesystem>
#include <cmath>

#include "BMPImageLib.h"
#include "Filter.h"

using namespace std;

static filesystem::path GetAssetPath(const string &filename)
{
    filesystem::path current = filesystem::path(__FILE__).parent_path();
    return current / "AssetsForTesting" / filename;
}

bool TestForBMPLoading()
{
    auto asset = GetAssetPath("Airplane.bmp");
    ifstream ifs(asset, ios::binary);
    if (!ifs.is_open())
    {
        cerr << "Failed to open asset: " << asset << '\n';
        return false;
    }

    BMPImage image;
    if (!image.read(ifs))
    {
        cerr << "BMP read failed for asset: " << asset << '\n';
        return false;
    }

    if (image.GetWidth() <= 0 || image.GetHeight() <= 0)
    {
        cerr << "Invalid dimensions: " << image.GetWidth() << "x" << image.GetHeight() << '\n';
        return false;
    }

    cout << "TestForBMPLoading: PASSED\n";
    return true;
}

bool TestForBMPSaving()
{
    auto asset = GetAssetPath("Airplane.bmp");
    ifstream ifs(asset, ios::binary);
    if (!ifs.is_open())
    {
        cerr << "Failed to open asset: " << asset << '\n';
        return false;
    }

    BMPImage image;
    if (!image.read(ifs))
    {
        cerr << "BMP read failed for asset: " << asset << '\n';
        return false;
    }

    auto outPath = GetAssetPath("Airplane_out_test.bmp");
    ofstream ofs(outPath, ios::binary);
    if (!ofs.is_open())
    {
        cerr << "Failed to open output file: " << outPath << '\n';
        return false;
    }

    image.write(ofs);
    ofs.close();

    ifstream ifs2(outPath, ios::binary | ios::ate);
    if (!ifs2.is_open())
    {
        cerr << "Failed to open written file: " << outPath << '\n';
        return false;
    }

    auto size = ifs2.tellg();
    if (size <= 0)
    {
        cerr << "Written file has size 0: " << outPath << '\n';
        filesystem::remove(outPath);
        return false;
    }

    filesystem::remove(outPath);
    cout << "TestForBMPSaving: PASSED\n";
    return true;
}

bool TestForGrayscaleFilter()
{
    auto asset = GetAssetPath("Airplane.bmp");
    ifstream ifs(asset, ios::binary);
    if (!ifs.is_open())
    {
        cerr << "Failed to open asset: " << asset << '\n';
        return false;
    }

    BMPImage image;
    if (!image.read(ifs))
    {
        cerr << "BMP read failed for asset: " << asset << '\n';
        return false;
    }

    GrayscaleFilter filter;
    filter.Apply(image);

    int width = image.GetWidth();
    int height = image.GetHeight();
    if (width <= 0 || height <= 0)
    {
        cerr << "Invalid dimensions after grayscale" << '\n';
        return false;
    }

    // Sample 10 random pixels (or full scan for small image) to verify grayscale
    const float tolerance = 0.001f;
    for (int y = 0; y < height; y += max(1, height / 10))
    {
        for (int x = 0; x < width; x += max(1, width / 10))
        {
            RGB p = image.GetPixel(x, y);
            if (fabs(p.r - p.g) > tolerance || fabs(p.g - p.b) > tolerance)
            {
                cerr << "Non-grayscale pixel found at (" << x << "," << y << "): " << p.r << "," << p.g << "," << p.b << '\n';
                return false;
            }
        }
    }

    cout << "TestForGrayscaleFilter: PASSED\n";
    return true;
}

int main()
{
    bool status = true;
    status &= TestForBMPLoading();
    status &= TestForBMPSaving();
    status &= TestForGrayscaleFilter();

    if (status)
    {
        cout << "All tests PASSED" << '\n';
        return 0;
    }

    cerr << "Some tests FAILED" << '\n';
    return 1;
}

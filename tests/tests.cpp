#include <iostream>
#include <cassert>
#include <fstream>
#include "../src/BMPImageLib.h"
#include "../src/Filter.h"

using namespace std;

void TestForBlurFilter() {
    BMPImage image;
    ifstream file("../../tests/AssetsForTesting/Airplane.bmp", ios::binary);
    assert(file.is_open() && "Failed to open file");
    bool loaded = image.read(file);
    file.close();
    assert(loaded && "Failed to load image");

    GaussianFilter filter(3.0);
    filter.Apply(image);

    // Check that the image is still valid
    assert(image.GetWidth() > 0 && image.GetHeight() > 0);

    // Check a few pixels to ensure they are reasonable
    RGB pixel = image.GetPixel(10, 10);
    assert(pixel.r >= 0.0f && pixel.r <= 1.0f);
    assert(pixel.g >= 0.0f && pixel.g <= 1.0f);
    assert(pixel.b >= 0.0f && pixel.b <= 1.0f);
    assert(pixel.a == 1.0f);

    cout << "Blur test passed" << endl;
}

int main() {
    TestForBlurFilter();
    cout << "All tests passed" << endl;
    return 0;
}

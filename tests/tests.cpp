#include <iostream>
#include <fstream>

#include "BMPImageLib.h"

using namespace std;

int TestForBMPLoading() {
    ifstream ifs_testing("C:\\Users\\Andrew\\Downloads\\BMPImaged (1)\\BMPImaged\\BMP_Filters_2\\Airplane.bmp", ios::binary);
    
    vector<std::byte> expected_r = {std::byte{255}, std::byte{0}, std::byte{0}};

    BMPImage img;
    if (!img.read(ifs_testing)) {
        return false;
    }

    ifs_testing.close();

    img.GetConstPixels() == exprected_r;

    img.ApplyFilter

}

void TestForBMPSaving() {
    
}

void TestForGrayscaleFilter() {
    
}

void TestForCropFilter() {
    
}

//...

int main() {
    return 0;
}
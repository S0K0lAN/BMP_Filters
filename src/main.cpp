#include <iostream>
#include <fstream>
    
#include "ActionManager.h"
#include "Filter.h"
#include "BMPImageLib.h"

using namespace std;

/*
{имя программы} {путь к входному файлу} {путь к выходному файлу}
[-{имя фильтра 1} [параметр фильтра 1] [параметр фильтра 2] ...]
[-{имя фильтра 2} [параметр фильтра 1] [параметр фильтра 2] ...] ...
*/
// void ReadAndProcessInput(std::istream& is, ActionManager& action_manager, int argc, char* argv[]) {
//     action_manager.LoadImage(argv[1]);
    
//     for (int i = 3; i < argc; i++) {
//         if(argv[i][0] == '-'){
//             FilterDescriptor filter_descriptor;
//             filter_descriptor.name = argv[i];
            

//             while(i + 1 < argc && argv[i+1][0] != '-') {
//                 filter_descriptor.parameters.push_back(argv[i + 1]);
//                 i++;
//             }

//             action_manager.ApplyFilter(filter_descriptor);
//         }
//     }

// }

// int main(int argc, char* argv[]){
//     ActionManager action_manager;
//     ReadAndProcessInput(cin, action_manager, argc, argv);
//     action_manager.SaveImage(argv[2]);



// }

int main() {
    std::ifstream ifs("C:\\Users\\Andrew\\Downloads\\BMPImaged (1)\\BMPImaged\\BMP_Filters_2\\Airplane.bmp", ios::binary);
    BMPImage img;
    img.read(ifs);
    std::ofstream ofs("Airplane_out.bmp", ios::binary);
    img.write(ofs);

    return 0;
}
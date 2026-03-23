#pragma once
#include <iostream>
#include <span>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

#include "BMPImageLib.h"
#include "Filter.h"

class ActionManager {
private:
    BMPImage image_;
    std::string input_file_;
    std::string output_file_;
    std::unordered_map<std::string, std::function<std::unique_ptr<IFilter>(const std::vector<std::string>&)>> filters_;
    
public:
    ActionManager();
    bool LoadImage(const std::string& path);
    bool SaveImage(const std::string& path);
    void ApplyFilter(const FilterDescriptor& filter_descriptor);
};

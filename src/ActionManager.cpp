#include "ActionManager.h"

#include <fstream>

ActionManager::ActionManager() {
    filters_["-crop"] = std::make_unique<CropFilter>();
    filters_["-gs"] = std::make_unique<GrayscaleFilter>();
    filters_["-neg"] = std::make_unique<NegativeFilter>();
    filters_["-sharp"] = std::make_unique<SharpeningFilter>();
    filters_["-blur"] = std::make_unique<GaussianFilter>();
    filters_["-edge"] = std::make_unique<EdgeDetectionFilter>();
}

bool ActionManager::LoadImage(const std::string& path) {
    std::ifstream ifs(path, std::ios_base::binary);
    if (!ifs.is_open()) {
        return false;
    }

    bool success = image_.read(ifs);
    ifs.close();
    
    if (success) {
        input_file_ = path;
    }
    
    return success;
}

bool ActionManager::SaveImage(const std::string& path) {
    std::ofstream ofs(path, std::ios_base::binary);
    if (!ofs.is_open()) {
        return false;
    }

    image_.write(ofs);
    ofs.close();
    
    output_file_ = path;
    return true;
}

void ActionManager::ApplyFilter(const FilterDescriptor& filter_descriptor) {
    auto it = filters_.find(filter_descriptor.name);
    if (it != filters_.end()) {
        it->second->Apply(image_);
    }
}
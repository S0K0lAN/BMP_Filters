#include "ActionManager.h"

#include <fstream>

#include "ActionManager.h"

#include <fstream>

ActionManager::ActionManager() {
    filters_["-crop"] = [](const std::vector<std::string>& params) -> std::unique_ptr<IFilter> {
        int w = 800, h = 600;
        if (params.size() >= 2) {
            w = std::stoi(params[0]);
            h = std::stoi(params[1]);
        }
        return std::make_unique<CropFilter>(w, h);
    };
    filters_["-gs"] = [](const std::vector<std::string>& params) -> std::unique_ptr<IFilter> {
        return std::make_unique<GrayscaleFilter>();
    };
    filters_["-neg"] = [](const std::vector<std::string>& params) -> std::unique_ptr<IFilter> {
        return std::make_unique<NegativeFilter>();
    };
    filters_["-sharp"] = [](const std::vector<std::string>& params) -> std::unique_ptr<IFilter> {
        return std::make_unique<SharpeningFilter>();
    };
    filters_["-blur"] = [](const std::vector<std::string>& params) -> std::unique_ptr<IFilter> {
        double s = 3.0;
        if (!params.empty()) {
            s = std::stod(params[0]);
        }
        return std::make_unique<GaussianFilter>(s);
    };
    filters_["-edge"] = [](const std::vector<std::string>& params) -> std::unique_ptr<IFilter> {
        double t = 0.1;
        if (!params.empty()) {
            t = std::stod(params[0]);
        }
        return std::make_unique<EdgeDetectionFilter>(t);
    };
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
        auto filter = it->second(filter_descriptor.parameters);
        filter->Apply(image_);
    }
}
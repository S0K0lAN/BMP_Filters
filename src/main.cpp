#include <iostream>
#include <fstream>
#include <string>

#include "ActionManager.h"

static void PrintUsage(const char *argv0)
{
    std::cerr << "Usage: " << argv0 << " <input.bmp> <output.bmp> [filters...]\n";
    std::cerr << "Filters:\n"
              << "  -gs      (grayscale)\n"
              << "  -neg     (negative)\n"
              << "  -crop width height (crop, default 800 600)\n"
              << "  -sharp   (sharpen)\n"
              << "  -blur sigma (gaussian blur, default sigma=3.0)\n"
              << "  -edge threshold (edge detection, default threshold=0.1)\n";
    std::cerr << "Example: " << argv0 << " input.bmp output.bmp -gs -neg\n";
}

static bool ReadAndProcessInput(ActionManager &action_manager, int argc, char *argv[])
{
    if (argc < 3)
    {
        PrintUsage(argv[0]);
        return false;
    }

    const std::string input_path = argv[1];
    const std::string output_path = argv[2];

    if (!action_manager.LoadImage(input_path))
    {
        std::cerr << "Failed to load image: " << input_path << '\n';
        return false;
    }

    for (int i = 3; i < argc; ++i)
    {
        std::string token = argv[i];
        if (token.empty() || token[0] != '-')
        {
            std::cerr << "Invalid argument: " << token << '\n';
            PrintUsage(argv[0]);
            return false;
        }

        FilterDescriptor descriptor;
        descriptor.name = token;

        while (i + 1 < argc && argv[i + 1][0] != '-')
        {
            descriptor.parameters.emplace_back(argv[++i]);
        }

        action_manager.ApplyFilter(descriptor);
    }

    if (!action_manager.SaveImage(output_path))
    {
        std::cerr << "Failed to save image: " << output_path << '\n';
        return false;
    }

    std::cout << "Image processed successfully: " << output_path << '\n';
    return true;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        PrintUsage(argv[0]);
        return 1;
    }

    ActionManager manager;
    if (!ReadAndProcessInput(manager, argc, argv))
    {
        return 1;
    }

    return 0;
}

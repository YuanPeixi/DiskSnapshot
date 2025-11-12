#pragma once
#include "pugixml.hpp"
#include <string>
#include <vector>
#include <ctime>

struct VHDXNode {
    std::string name;
    std::string desc;
    time_t timeSnap;
    std::string path;
    std::string parent;
};

bool LoadVHDXConfig(const std::string& xmlPath, std::vector<VHDXNode>& nodes);
bool SaveVHDXConfig(const std::string& xmlPath, const std::vector<VHDXNode>& nodes);
std::string FormatTime(time_t t); // תΪ "YYYY-MM-DD HH:MM"


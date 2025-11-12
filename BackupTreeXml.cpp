#include "BackupTreeXml.h"
#include <iomanip>
#include <sstream>

std::string FormatTime(time_t t)
{
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return oss.str();
}

bool LoadVHDXConfig(const std::string& xmlPath, std::vector<VHDXNode>& nodes)
{
    pugi::xml_document doc;
    if (!doc.load_file(xmlPath.c_str())) return false;
    auto root = doc.child("systems");
    for (auto n : root.children("vhdx_node")) {
        VHDXNode node;
        node.name = n.attribute("name").as_string();
        node.desc = n.attribute("desc").as_string();
        node.path = n.attribute("path").as_string();
        node.parent = n.attribute("parent").as_string();
        node.timeSnap = n.attribute("timeSnap").as_llong();
        nodes.push_back(node);
    }
    return true;
}

bool SaveVHDXConfig(const std::string& xmlPath, const std::vector<VHDXNode>& nodes)
{
    pugi::xml_document doc;
    auto root = doc.append_child("systems");
    for (const auto& n : nodes) {
        auto x = root.append_child("vhdx_node");
        x.append_attribute("name") = n.name.c_str();
        x.append_attribute("desc") = n.desc.c_str();
        x.append_attribute("path") = n.path.c_str();
        x.append_attribute("parent") = n.parent.c_str();
        x.append_attribute("timeSnap") = (long long)n.timeSnap;
    }
    return doc.save_file(xmlPath.c_str());
}
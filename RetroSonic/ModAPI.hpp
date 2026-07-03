#ifndef MOD_API_H
#define MOD_API_H

#if RETRO_USE_MOD_LOADER
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

struct ModInfo {
    std::string name;
    std::string desc;
    std::string author;
    std::string version;
    std::map<std::string, std::string> fileMap;
    std::string folder;
    bool skipStartMenu;
    bool active;
};

extern std::vector<ModInfo> ModList;

void InitMods();

#endif // !RETRO_USE_MOD_LOADER

#endif // !MOD_API_H
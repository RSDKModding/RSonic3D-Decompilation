#include "RetroEngine.hpp"

#if !RETRO_USE_ORIGINAL_CODE && RETRO_USE_MOD_LOADER
#include <filesystem>
#include <locale>

std::vector<ModInfo> ModList;

namespace fs = std::filesystem;

fs::path ResolvePath(fs::path given)
{
    if (given.is_relative())
        given = fs::current_path() / given;

    for (auto &p : fs::directory_iterator{ given.parent_path() }) {
        char pbuf[0x100];
        char gbuf[0x100];

        auto pf   = p.path().filename();
        auto pstr = pf.string();
        StringLowerCase(pbuf, pstr.c_str());

        auto gf   = given.filename();
        auto gstr = gf.string();
        StringLowerCase(gbuf, gstr.c_str());

        if (StrComp(pbuf, gbuf))
            return p.path();
    }

    return given;
}

#if 0
void InitMods()
{
    ModList.clear();

    char modBuf[0x100];
    sprintf(modBuf, "%smods", modsPath);

    fs::path modPath = ResolvePath(modBuf);

    if (fs::exists(modPath) && fs::is_directory(modPath)) {
        std::string mod_config = modPath.string() + "/modconfig.ini";
        FileIO *configFile     = fOpen(mod_config.c_str(), "r");
        if (configFile) {
            fClose(configFile);
            IniParser modConfig(mod_config.c_str(), false);

            for (int m = 0; m < modConfig.items.size(); ++m) {
                bool active = false;
                ModInfo info;
                modConfig.GetBool("mods", modConfig.items[m].key, &active);
                if (LoadMod(&info, modPath.string(), modConfig.items[m].key, active))
                    ModList.push_back(info);
            }
        }

        try {
            auto rdi = fs::directory_iterator(modPath);
            for (auto de : rdi) {
                if (de.is_directory()) {
                    fs::path modDirPath = de.path();

                    ModInfo info;

                    std::string modDir            = modDirPath.string().c_str();
                    const std::string mod_inifile = modDir + "/mod.ini";
                    std::string folder            = modDirPath.filename().string();

                    bool flag = true;
                    for (int m = 0; m < ModList.size(); ++m) {
                        if (ModList[m].folder == folder) {
                            flag = false;
                            break;
                        }
                    }

                    if (flag) {
                        if (LoadMod(&info, modPath.string(), modDirPath.filename().string(), false))
                            ModList.push_back(info);
                    }
                }
            }
        } catch (fs::filesystem_error fe) {
            // oh well
        }
    }

    skipStartMenu = skipStartMenu_Config;
    for (int m = 0; m < ModList.size(); ++m) {
        if (!ModList[m].active)
            continue;
        if (ModList[m].skipStartMenu)
            skipStartMenu = true;
    }
}
#endif

#endif // !RETRO_USE_MOD_LOADER
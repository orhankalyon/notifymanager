#include "config.hpp"

vector<string> configModules(string NM_CONFIG_DIR) {
    vector<string> modules;

    auto config = toml::parse_file(NM_CONFIG_DIR + "/config.toml");
    auto rawModules = config["generic"]["modules"];

    for (int i = 0; i < !!rawModules; i++) {
        modules.push_back(rawModules[i].as_string()->get());
    }

    return modules;
}

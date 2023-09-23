#include <iostream>
#include <string>
#include "util.hpp"
#include "config.hpp"
#include "database.hpp" 

int main(int argc, char** argv) {
    string HOME;
    string NM_CONFIG_DIR;
    string NM_CONFIG_DIR_DEFAULT;

    string dbPath;
    string query;


    HOME = getEnvVar("HOME");
    NM_CONFIG_DIR = getEnvVar("NM_CONFIG_DIR");
    NM_CONFIG_DIR_DEFAULT = HOME + "/.config/notifymanager";

    if (NM_CONFIG_DIR == "") {
        NM_CONFIG_DIR = NM_CONFIG_DIR_DEFAULT;
    }

    
    vector<string> modules = configModules(NM_CONFIG_DIR);

    for (int i = 0; i < modules.size(); i++) {
        string extension;
        string command;

        extension = get_extension(modules[i]);

        if (extension == ".sh") {
            command = "bash " + NM_CONFIG_DIR + "/modules/" + modules[i];
        } else if (extension == ".py") {
            command = "python " + NM_CONFIG_DIR + "/modules/" + modules[i];
        } else if (extension == ".js") {
            command = "node " + NM_CONFIG_DIR + "/modules/" + modules[i];
        } else if (extension == ".lua") {
            command = "lua " + NM_CONFIG_DIR + "/modules/" + modules[i];
        } else {
            return 1;
        }

        system(command.c_str());
    }


    dbPath = NM_CONFIG_DIR + "/notifications.db";


    Database database = Database(dbPath.c_str());

    query = "SELECT * FROM new;";
    database.select_all(query);

    query = "DELETE FROM new;";
    database.exec(query);
}

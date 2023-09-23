#include "util.hpp"

string getEnvVar(string const &key) {
    char *val = getenv(key.c_str());

    return val == NULL ? string("") : string(val);
}


string get_extension(const string& input) {
    regex rgx("\\.[^.]+$");
    smatch match;

    if (regex_search(input.begin(), input.end(), match, rgx)) {
        return match[0].str();
    }

    return "";
}

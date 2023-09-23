#include "database.hpp"

Database::Database(const char *path) {
    sqlite3_open(path, &DB);
    string query = "CREATE TABLE IF NOT EXISTS new(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, count INTEGER);";
    sqlite3_exec(DB, query.c_str(), 0, 0, 0);
}

int Database::exec(string query) {
    int result;

    result = sqlite3_exec(DB, query.c_str(), 0, 0, 0);

    return result;
}

int Database::select_all(string query) {
    int result;

    result = sqlite3_exec(DB, query.c_str(), this->callback, 0, 0);

    return 0;
}

int Database::callback(void *data, int argc, char **argv, char **azColName) {
    const char *name = argv[1] ? argv[1] : "NULL";
    const char *count = argv[2] ? argv[2] : "NULL";

    cout << name << ": \e[0;32m" << count << "\e[0m" << endl;

    return 0;
}

Database::~Database() {
    sqlite3_close(DB);
}

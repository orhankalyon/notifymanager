#include <iostream>
#include <vector>
#include <sqlite3.h>

using namespace std;

class Database {
    private:        
        sqlite3 *DB;

        static int callback(void *data, int argc, char **argv, char **azColName);

    public:
        Database(const char *path);
        int exec(string query);
        int select_all(string query);
        ~Database();
};

#include <iostream>
#include <string>
#include "../sqlite3.h"

using namespace std;

int main() {
    sqlite3* db;
    char* errMsg = 0;

    int rc = sqlite3_open("db/tasks.db", &db);
    if (rc) {
        cout << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return 1;
    }
    cout << "Database opened successfully." << endl;

    string sql = "CREATE TABLE IF NOT EXISTS tasks ("
                 "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "title    TEXT NOT NULL,"
                 "due_date TEXT,"
                 "priority TEXT DEFAULT 'Medium',"
                 "status   TEXT DEFAULT 'Pending'"
                 ");";

    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Tasks table ready." << endl;
    }

    sqlite3_close(db);
    return 0;
}
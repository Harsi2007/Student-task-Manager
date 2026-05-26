#include <iostream>
#include <string>
#include "../sqlite3.h"
using namespace std;
sqlite3* db;
void openDB() {
    int rc = sqlite3_open("db/tasks.db", &db);
    if (rc) {
        cout << "Cannot open database!" << endl;
        exit(1);
    }
    string sql = "CREATE TABLE IF NOT EXISTS tasks ("
                 "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "title    TEXT NOT NULL,"
                 "due_date TEXT,"
                 "priority TEXT DEFAULT 'Medium',"
                 "status   TEXT DEFAULT 'Pending'"
                 ");";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
}
static int printRow(void* data, int argc, char** argv, char** colName) {
    cout << "\nID: " << argv[0];
    cout << "\nTitle: " << argv[1];
    cout << "\nDue Date: " << argv[2];
    cout << "\nPriority: " << argv[3];
    cout << "\nStatus: " << argv[4];
    cout << "\n-------------------";
    return 0;
}
void addTask() {
    string title;
    string dueDate;
    string priority;

    cout << "Enter task title: ";
    cin.ignore();
    getline(cin, title);

    cout << "Enter due date (e.g. 2026-06-01): ";
    getline(cin, dueDate);

    cout << "Enter priority (High/Medium/Low): ";
    getline(cin, priority);

    string sql = "INSERT INTO tasks (title, due_date, priority) VALUES ('"
                 + title + "', '" + dueDate + "', '" + priority + "');";

    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
    cout << "Task added successfully!" << endl;
}
void viewAllTasks() {
    cout << "\n===== ALL TASKS =====";
    string sql = "SELECT * FROM tasks;";
    sqlite3_exec(db, sql.c_str(), printRow, 0, 0);
    cout << endl;
}
void deleteTask() {
    int id;
    cout << "Enter task ID to delete: ";
    cin >> id;
    string sql = "DELETE FROM tasks WHERE id = " + to_string(id) + ";";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
    cout << "Task deleted!" << endl;
}
void markDone() {
    int id;
    cout << "Enter task ID to mark as done: ";
    cin >> id;
    string sql = "UPDATE tasks SET status = 'Done' WHERE id = " + to_string(id) + ";";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
    cout << "Task marked as done!" << endl;
}
int main() {
    openDB();
    int choice;
    do {
        cout << "\n===== TASK MANAGER =====";
        cout << "\n1. Add task";
        cout << "\n2. View all tasks";
        cout << "\n3. Mark task done";
        cout << "\n4. Delete task";
        cout << "\n5. Exit";
        cout << "\nChoose: ";
        cin >> choice;

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewAllTasks(); break;
            case 3: markDone(); break;
            case 4: deleteTask(); break;
            case 5: cout << "Bye!" << endl; break;
            default: cout << "Invalid choice!" << endl;
        }
    } while (choice != 5);

    sqlite3_close(db);
    return 0;
}
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
    int* count = (int*)data;
    (*count)++;
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

    cout << "Enter due date (eg: yyyy-mm-dd): ";
    getline(cin, dueDate);

    cout << "Enter priority (High/Medium/Low): ";
    getline(cin, priority);
    priority[0] = toupper(priority[0]);
    for (int i = 1; i < priority.length(); i++) {
        priority[i] = tolower(priority[i]);
    }

    string sql = "INSERT INTO tasks (title, due_date, priority) VALUES ('"
                 + title + "', '" + dueDate + "', '" + priority + "');";

    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
    cout << "Task added successfully!" << endl;
}
void viewAllTasks() {
    cout << "\n===== ALL TASKS =====";
    int count = 0;
    string sql = "SELECT * FROM tasks;";
    sqlite3_exec(db, sql.c_str(), printRow, &count, 0);
    if (count == 0) cout << "\nNo tasks found!" << endl;
    cout << endl;
}
void deleteTask() {
    int id;
    cout << "Enter task ID to delete: ";
    cin >> id;
    string sql = "DELETE FROM tasks WHERE id = " + to_string(id) + ";";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
    if (sqlite3_changes(db) == 0) {
        cout << "No task found with ID " << id << "!" << endl;
    } else {
        cout << "Task deleted!" << endl;
    }
}
void markDone() {
    int id;
    cout << "Enter task ID to mark as done: ";
    cin >> id;
    string sql = "UPDATE tasks SET status = 'Done' WHERE id = " + to_string(id) + ";";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
    if (sqlite3_changes(db) == 0) {
        cout << "No task found with ID " << id << "!" << endl;
    } else {
        cout << "Task marked as done!" << endl;
    }
}
void viewPending() {
    cout << "\n===== PENDING TASKS =====";
    int count = 0;
    string sql = "SELECT * FROM tasks WHERE status = 'Pending';";
    sqlite3_exec(db, sql.c_str(), printRow, &count, 0);
    if (count == 0) cout << "\nNo pending tasks!" << endl;
    cout << endl;
}

void viewByPriority() {
    string priority;
    cout << "Enter priority (High/Medium/Low): ";
    cin.ignore();
    getline(cin, priority);
    priority[0] = toupper(priority[0]);
    for (int i = 1; i < priority.length(); i++) {
        priority[i] = tolower(priority[i]);
    }
    int count = 0;
    string sql = "SELECT * FROM tasks WHERE priority = '" + priority + "';";
    sqlite3_exec(db, sql.c_str(), printRow, &count, 0);
    if (count == 0) cout << "\nNo tasks with " << priority << " priority!" << endl;
    cout << endl;
}
void searchTask() {
    string keyword;
    cout << "Enter search keyword: ";
    cin.ignore();
    getline(cin, keyword);
    int count = 0;
    string sql = "SELECT * FROM tasks WHERE title LIKE '%" + keyword + "%';";
    sqlite3_exec(db, sql.c_str(), printRow, &count, 0);
    if (count == 0) cout << "\nNo tasks found!" << endl;
    cout << endl;
}
int main() {
    openDB();
    int choice;
    do {
        cout << "\n========================";
        cout << "\n     TASK MANAGER       ";
        cout << "\n========================";
        cout << "\n 1. Add task";
        cout << "\n 2. View all tasks";
        cout << "\n 3. Mark task done";
        cout << "\n 4. Delete task";
        cout << "\n 5. View pending tasks";
        cout << "\n 6. View by priority";
        cout << "\n 7. Search task";
        cout << "\n 8. Exit";
        cout << "\n========================";
        cout << "\nChoose: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = 0;
        }

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewAllTasks(); break;
            case 3: markDone(); break;
            case 4: deleteTask(); break;
            case 5: viewPending(); break; 
            case 6: viewByPriority(); break;
            case 7: searchTask(); break;
            case 8: cout << "Bye!" << endl; break;
            default: cout << "Invalid choice!" << endl;
        }
    } while (choice != 8);

    sqlite3_close(db);
    return 0;
}
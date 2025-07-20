#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>

using namespace std;

// ----------------------- USER LOGIN -----------------------

unordered_map<string, string> userDatabase = {
    {"mihikadesai@gmail.com", "mypassword"},
    {"admin@admin.com", "admin123"}
};

bool authenticateUser(const string& email, const string& password) {
    return userDatabase.find(email) != userDatabase.end() && userDatabase[email] == password;
}

void displayWorkspace(const string& username) {
    cout << "\n=====================================\n";
    cout << "Welcome, " << username << "!" << endl;
    cout << "Your tailored workspace is now configured.\n";
    cout << "Here are your options:\n";
    cout << "1. View Calendar\n";
    cout << "2. Manage To-Do List\n";
    cout << "3. Add Event\n";
    cout << "4. Exit\n";
}

// ----------------------- CALENDAR -----------------------

unordered_map<string, string> eventDatabase;

int dayNumber(int day, int month, int year) {
    static int A[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= month < 3;
    return (year + year / 4 - year / 100 + year / 400 + A[month - 1] + day) % 7;
}

string getMonthName(int monthNumber) {
    string months[] = {"January", "February", "March", "April", "May", "June",
                       "July", "August", "September", "October", "November", "December"};
    return months[monthNumber];
}

int numberOfDays(int month, int year) {
    switch (month) {
        case 0: case 2: case 4: case 6: case 7: case 9: case 11: return 31;
        case 1: return (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) ? 29 : 28;
        case 3: case 5: case 8: case 10: return 30;
        default: return 0;
    }
}

void addEvent() {
    string date, event;
    cout << "Enter the date (DD-MM-YYYY): ";
    cin >> date;
    cin.ignore();
    cout << "Enter the event description: ";
    getline(cin, event);
    eventDatabase[date] = event;
    cout << "✅ Event added for " << date << ": " << event << endl;
}

void printCalendar(int year) {
    cout << "\n\t\tCalendar - Year " << year << "\n";
    int current = dayNumber(1, 1, year);

    for (int i = 0; i < 12; i++) {
        int days = numberOfDays(i, year);
        cout << "\n\t\t" << getMonthName(i) << "\n";
        cout << "Sun Mon Tue Wed Thu Fri Sat\n";

        for (int k = 0; k < current; k++) cout << "    ";

        for (int j = 1; j <= days; j++) {
            string date = (j < 10 ? "0" : "") + to_string(j) + "-" +
                          (i + 1 < 10 ? "0" : "") + to_string(i + 1) + "-" +
                          to_string(year);

            if (eventDatabase.find(date) != eventDatabase.end())
                cout << (j < 10 ? " " : "") << j << "* ";
            else
                cout << (j < 10 ? " " : "") << j << "  ";

            if (++current > 6) {
                current = 0;
                cout << endl;
            }
        }
        if (current != 0) cout << endl;
    }
}

// ----------------------- TO-DO LIST -----------------------

class Task {
private:
    string name;
    string dueDate;
    bool completed;

public:
    Task(const string& taskName, const string& taskDueDate)
        : name(taskName), dueDate(taskDueDate), completed(false) {}

    void markCompleted() { completed = true; }

    void displayTask() const {
        cout << name << " (" << (completed ? "Completed" : "Pending") << ") - Due: " << dueDate << endl;
    }

    string getName() const { return name; }
};

class ToDoList {
private:
    vector<Task> tasks;

    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    void displayMenu() {
        cout << "\n--- To-Do List Menu ---\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Mark Task as Completed\n";
        cout << "4. Back to Main Menu\n";
    }

    void addTask() {
        string name, due;
        cout << "Enter task name: ";
        cin >> ws;
        getline(cin, name);
        cout << "Enter due date (DD-MM-YYYY): ";
        getline(cin, due);
        tasks.emplace_back(name, due);
        cout << "✅ Task added!\n";
    }

    void viewTasks() {
        if (tasks.empty()) {
            cout << "No tasks.\n";
            return;
        }
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << i + 1 << ". ";
            tasks[i].displayTask();
        }
    }

    void completeTask() {
        if (tasks.empty()) {
            cout << "No tasks to complete.\n";
            return;
        }
        viewTasks();
        int choice;
        cout << "Enter task number to complete: ";
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > (int)tasks.size()) {
            clearInputBuffer();
            cout << "Invalid choice.\n";
            return;
        }
        tasks[choice - 1].markCompleted();
        tasks.erase(tasks.begin() + (choice - 1));
        cout << "✅ Task marked as completed and removed!\n";
    }

    void run() {
        int choice;
        do {
            displayMenu();
            cout << "Choose: ";
            cin >> choice;

            switch (choice) {
                case 1: addTask(); break;
                case 2: viewTasks(); break;
                case 3: completeTask(); break;
                case 4: break;
                default: cout << "Invalid option.\n";
            }
        } while (choice != 4);
    }
};

// ----------------------- MAIN PROGRAM -----------------------

int main() {
    string email, password;

    cout << "Welcome to ToDoOrNot - Terminal Scheduler 🗓️\n";
    cout << "Enter email: ";
    getline(cin, email);
    cout << "Enter password: ";
    getline(cin, password);

    if (!authenticateUser(email, password)) {
        cout << "❌ Invalid credentials.\n";
        return 1;
    }

    displayWorkspace(email);

    ToDoList todo;
    int choice;

    do {
        cout << "\nMain Menu:\n";
        cout << "1. View Calendar\n2. Manage To-Do List\n3. Add Event\n4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int year;
                cout << "Enter year: ";
                cin >> year;
                printCalendar(year);
                break;
            }
            case 2:
                todo.run();
                break;
            case 3:
                addEvent();
                break;
            case 4:
                cout << "👋 Exiting. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 4);

    return 0;
}

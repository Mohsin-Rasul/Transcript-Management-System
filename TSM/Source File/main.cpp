#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <limits>

using namespace std;

const int MAX_STUDENTS = 20;
const int MAX_SUBJECTS = 5;
const int SEMESTERS = 2;
const int MAX_NAME_LENGTH = 50;
const int MAX_ID_LENGTH = 20;

struct Subject {
    char name[MAX_NAME_LENGTH];
    char grade;
};

struct Semester {
    Subject subjects[MAX_SUBJECTS];
    float gpa;
};

struct Student {
    char id[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    Semester semesters[SEMESTERS];
    float cgpa;
};

struct User {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
};

vector<Student> students;
vector<User> users;
bool isLoggedIn = false;

float gradeToPoints(char grade) {
    switch (toupper(grade)) {
    case 'A': return 4.0;
    case 'B': return 3.0;
    case 'C': return 2.0;
    case 'D': return 1.0;
    default: return 0.0;
    }
}

void displayMenu();
void addStudent();
void enterGrades();
void calculateGPA(Semester& semester);
void calculateCGPA(Student& student);
void generateTranscript();
void saveToFile();
void loadFromFile();
void searchStudent();
void loadUsers();
void saveUsers();
bool login();
void signup();

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    int choice;

    loadUsers();
    loadFromFile();

    cout << "Welcome to the Transcript Management System\n";
    while (!isLoggedIn) {
        cout << "1. Login\n2. Signup\n3. Exit\nEnter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1:
            if (login()) {
                cout << "Login successful.\n";
                isLoggedIn = true;
            }
            else {
                cout << "Invalid credentials. Try again.\n";
            }
            break;
        case 2:
            signup();
            break;
        case 3:
            cout << "Exiting program.\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    do {
        displayMenu();
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1:
            addStudent();
            saveToFile();
            break;
        case 2:
            enterGrades();
            saveToFile();
            break;
        case 3:
            generateTranscript();
            break;
        case 4:
            searchStudent();
            break;
        case 5:
            cout << "Exiting program.\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

void displayMenu() {
    cout << "\nTranscript Management System\n";
    cout << "1. Add Student\n";
    cout << "2. Enter Grades\n";
    cout << "3. Generate Transcript\n";
    cout << "4. Search Student\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

void addStudent() {
    if (students.size() < MAX_STUDENTS) {
        Student newStudent;
        cout << "Enter student ID (e.g., bcy243024): ";
        cin.getline(newStudent.id, MAX_ID_LENGTH);
        cout << "Enter student name: ";
        cin.getline(newStudent.name, MAX_NAME_LENGTH);

        for (int sem = 0; sem < SEMESTERS; ++sem) {
            for (int subj = 0; subj < MAX_SUBJECTS; ++subj) {
                cout << "Enter subject " << subj + 1 << " name for semester " << sem + 1 << ": ";
                cin.getline(newStudent.semesters[sem].subjects[subj].name, MAX_NAME_LENGTH);
                newStudent.semesters[sem].subjects[subj].grade = 'N';
            }
        }
        newStudent.cgpa = 0;
        students.push_back(newStudent);
        cout << "Student added successfully.\n";
    }
    else {
        cout << "Maximum number of students reached.\n";
    }
}

void enterGrades() {
    char id[MAX_ID_LENGTH];
    cout << "Enter student ID: ";
    cin.getline(id, MAX_ID_LENGTH);

    for (auto& student : students) {
        if (strcmp(student.id, id) == 0) {
            for (int sem = 0; sem < SEMESTERS; ++sem) {
                cout << "Enter grades for semester " << sem + 1 << ":\n";
                for (int subj = 0; subj < MAX_SUBJECTS; ++subj) {
                    cout << student.semesters[sem].subjects[subj].name << " (A/B/C/D/F): ";
                    cin >> student.semesters[sem].subjects[subj].grade;
                    clearInputBuffer();
                }
                calculateGPA(student.semesters[sem]);
            }
            calculateCGPA(student);
            cout << "Grades entered successfully.\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void calculateGPA(Semester& semester) {
    float sum = 0;
    int count = 0;
    for (int i = 0; i < MAX_SUBJECTS; ++i) {
        if (semester.subjects[i].grade != 'N') {
            sum += gradeToPoints(semester.subjects[i].grade);
            count++;
        }
    }
    semester.gpa = (count > 0) ? sum / count : 0;
}

void calculateCGPA(Student& student) {
    float sum = 0;
    for (int i = 0; i < SEMESTERS; ++i) {
        sum += student.semesters[i].gpa;
    }
    student.cgpa = sum / SEMESTERS;
}

void generateTranscript() {
    char id[MAX_ID_LENGTH];
    cout << "Enter student ID: ";
    cin.getline(id, MAX_ID_LENGTH);

    for (const auto& student : students) {
        if (strcmp(student.id, id) == 0) {
            cout << "\n" << string(50, '=') << endl;
            cout << "Transcript for " << student.name << " (ID: " << student.id << ")" << endl;
            cout << string(50, '=') << endl;

            for (int sem = 0; sem < SEMESTERS; ++sem) {
                cout << "\nSemester " << sem + 1 << ":" << endl;
                cout << left << setw(30) << "Subject" << setw(10) << "Grade" << endl;
                cout << string(40, '-') << endl;
                for (int subj = 0; subj < MAX_SUBJECTS; ++subj) {
                    if (student.semesters[sem].subjects[subj].grade != 'N') {
                        cout << left << setw(30) << student.semesters[sem].subjects[subj].name
                            << setw(10) << student.semesters[sem].subjects[subj].grade << endl;
                    }
                }
                cout << string(40, '-') << endl;
                cout << left << setw(30) << "GPA:" << fixed << setprecision(2) << student.semesters[sem].gpa << endl;
                cout << string(40, '-') << endl;
            }
            cout << "\n" << left << setw(30) << "CGPA:" << fixed << setprecision(2) << student.cgpa << endl;
            cout << string(50, '=') << endl;
            return;
        }
    }
    cout << "Student not found.\n";
}

void saveToFile() {
    ofstream outFile("students.dat", ios::binary);
    if (outFile.is_open()) {
        size_t size = students.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        outFile.write(reinterpret_cast<const char*>(students.data()), size * sizeof(Student));
        outFile.close();
    }
    else {
        cout << "Unable to open file for writing.\n";
    }
}

void loadFromFile() {
    ifstream inFile("students.dat", ios::binary);
    if (inFile.is_open()) {
        size_t size;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        students.resize(size);
        inFile.read(reinterpret_cast<char*>(students.data()), size * sizeof(Student));
        inFile.close();
        cout << "Data loaded from file.\n";
    }
    else {
        cout << "No existing data file found. Starting with empty database.\n";
    }
}

void searchStudent() {
    char id[MAX_ID_LENGTH];
    cout << "Enter student ID to search: ";
    cin.getline(id, MAX_ID_LENGTH);

    for (const auto& student : students) {
        if (strcmp(student.id, id) == 0) {
            cout << "\nStudent found:\n";
            cout << "Name: " << student.name << "\n";
            cout << "ID: " << student.id << "\n";
            cout << "CGPA: " << fixed << setprecision(2) << student.cgpa << "\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void loadUsers() {
    ifstream inFile("users.dat", ios::binary);
    if (inFile.is_open()) {
        size_t size;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        users.resize(size);
        inFile.read(reinterpret_cast<char*>(users.data()), size * sizeof(User));
        inFile.close();
    }
    else {
        cout << "No existing user file found.\n";
    }
}

void saveUsers() {
    ofstream outFile("users.dat", ios::binary);
    if (outFile.is_open()) {
        size_t size = users.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        outFile.write(reinterpret_cast<const char*>(users.data()), size * sizeof(User));
        outFile.close();
    }
    else {
        cout << "Unable to save user file.\n";
    }
}

bool login() {
    char username[MAX_NAME_LENGTH], password[MAX_NAME_LENGTH];
    cout << "Enter username: ";
    cin.getline(username, MAX_NAME_LENGTH);
    cout << "Enter password: ";
    cin.getline(password, MAX_NAME_LENGTH);

    for (const auto& user : users) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            return true;
        }
    }
    return false;
}

void signup() {
    User newUser;
    cout << "Enter new username: ";
    cin.getline(newUser.username, MAX_NAME_LENGTH);
    cout << "Enter new password: ";
    cin.getline(newUser.password, MAX_NAME_LENGTH);
    users.push_back(newUser);
    saveUsers();
    cout << "Signup successful.\n";
}
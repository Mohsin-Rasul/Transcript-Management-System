#include<iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

const int maxStudents = 20;
const int maxSubjects = 5;
const int maxSem = 2;
const int maxDepart = 2;
const int maxPrograms = 3;
const int maxNameL = 30;
const int maxIdL = 9;
const int maxUsers= 20;

struct Subject {
    char name[maxNameL];
    char grade;
};

struct Semester {
    Subject subjects[maxSubjects];
    float gpa;
};

struct Student {
    char id[maxIdL];
    char name[maxNameL];
    char department[maxDepart];
    char course[maxNameL];
    Semester maxSem[maxSem];
    float cgpa;
};

struct User {
    char username[maxNameL];
    char password[maxNameL];
};

Student students[maxStudents];
User users[maxUsers];
int studentCount = 0;
int userCount = 0;
bool isLoggedIn = false;

const char departments[maxDepart][maxNameL] = {
    "Cyber Security", "Electrical Engineering"
};

const char courses[maxDepart][maxPrograms][maxNameL] = {
    {"Cybersecurity", "Software Engineering", "Data Science"},
    {"Power Systems", "Electronics", "Control Systems"},
    {"Thermodynamics", "Robotics", "Fluid Mechanics"}
};

const char subjects[maxDepart][maxPrograms][maxSem][maxSubjects][maxNameL] = {
    { // Computer Science
        { // Cybersecurity
            {"Intro to Cybersecurity", "Programming Fundamentals", "Discrete Mathematics", "Digital Logic", "Communication Skills"},
            {"Data Structures", "Operating Systems", "Network Security", "DBMS", "Professional Ethics"}
        },
        { // Software Engineering
            {"Programming Fundamentals", "Discrete Mathematics", "Digital Logic", "Communication Skills"},
            {"Data Structures", "Operating Systems", "Software Design", "DBMS", "Team Collaboration"}
        },
        { // Data Science
            {"Intro to Data Science", "Programming Fundamentals", "Linear Algebra", "Digital Logic", "Communication Skills"},
            {"Data Structures", "Statistics", "Machine Learning", "DBMS", "Visualization Techniques"}
        }
    },
    { // Electrical Engineering
        { // Power Systems
            {"Intro to Power Systems", "Circuit Analysis", "Electromagnetics", "Digital Logic", "Communication Skills"},
            {"Power Electronics", "Power Systems Analysis", "Control Systems", "DBMS", "Professional Ethics"}
        },
        { // Electronics
            {"Intro to Electronics", "Circuit Analysis", "Electromagnetics", "Digital Logic", "Communication Skills"},
            {"Analog Electronics", "Digital Electronics", "Control Systems", "DBMS", "Professional Ethics"}
        },
        { // Control Systems
            {"Intro to Control Systems", "Circuit Analysis", "Electromagnetics", "Digital Logic", "Communication Skills"},
            {"Control Systems Design", "Signals and Systems", "Process Control", "DBMS", "Robotics"}
        }
    },
    { // Mechanical Engineering
        { // Thermodynamics
            {"Intro to Thermodynamics", "Mechanics", "Material Science", "Digital Logic", "Communication Skills"},
            {"Fluid Mechanics", "Thermal Systems", "Heat Transfer", "DBMS", "Energy Systems"}
        },
        { // Robotics
            {"Intro to Robotics", "Mechanics", "Material Science", "Digital Logic", "Communication Skills"},
            {"Advanced Robotics", "Control Systems", "Machine Dynamics", "DBMS", "Automation"}
        },
        { // Fluid Mechanics
            {"Intro to Fluid Mechanics", "Mechanics", "Material Science", "Digital Logic", "Communication Skills"},
            {"Advanced Fluid Mechanics", "Hydraulics", "Heat Transfer", "DBMS", "Energy Systems"}
        }
    }
};

void displayMenu();
void addStudent();
void enterGrades();
void calculateGPA(Semester& semester);
void calculateCGPA(Student& student);
void generateTranscript();
void saveToFile();
void loadFromFile();
void searchStudent();
void clearInputBuffer();
void loadUsers();
void saveUsers();
bool login();
void signup();

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
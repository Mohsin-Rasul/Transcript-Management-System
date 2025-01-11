#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

const int MAX_STUDENTS = 20;
const int MAX_SUBJECTS = 5;
const int SEMESTERS = 2;
const int MAX_NAME_LENGTH = 50;
const int MAX_ID_LENGTH = 20;
const int MAX_USERS = 20;
const int MAX_DEPARTMENTS = 3;
const int MAX_COURSES = 3;

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
    char department[MAX_NAME_LENGTH];
    char course[MAX_NAME_LENGTH];
    Semester semesters[SEMESTERS];
    float cgpa;
};

struct User {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
};

Student students[MAX_STUDENTS];
User users[MAX_USERS];
int studentCount = 0;
int userCount = 0;
bool isLoggedIn = false;

// Predefined subjects for departments and courses
const char departments[MAX_DEPARTMENTS][MAX_NAME_LENGTH] = {
    "Cyber Security", "Electrical Engineering", "Mechanical Engineering"
};

const char courses[MAX_DEPARTMENTS][MAX_COURSES][MAX_NAME_LENGTH] = {
    {"Cybersecurity", "Software Engineering", "Data Science"},
    {"Power Systems", "Electronics", "Control Systems"},
    {"Thermodynamics", "Robotics", "Fluid Mechanics"}
};

const char subjects[MAX_DEPARTMENTS][MAX_COURSES][SEMESTERS][MAX_SUBJECTS][MAX_NAME_LENGTH] = {
    { // Computer Science
        { // Cybersecurity
            {"Intro to Cybersecurity", "Programming Fundamentals", "Discrete Mathematics", "Digital Logic", "Communication Skills"},
            {"Data Structures", "Operating Systems", "Network Security", "DBMS", "Professional Ethics"}
        },
        { // Software Engineering
            {"Software Engineering Fundamentals", "Programming Fundamentals", "Discrete Mathematics", "Digital Logic", "Communication Skills"},
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

void clearInputBuffer() {
    cin.clear();
    cin.ignore(1000, '\n');
}

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        cout << "Maximum number of students reached.\n";
        return;
    }

    Student& newStudent = students[studentCount];
    cout << "Enter student ID: ";
    cin.getline(newStudent.id, MAX_ID_LENGTH);
    cout << "Enter student name: ";
    cin.getline(newStudent.name, MAX_NAME_LENGTH);

    cout << "Available Departments:\n";
    for (int i = 0; i < MAX_DEPARTMENTS; i++) {
        cout << i + 1 << ". " << departments[i] << "\n";
    }
    int deptChoice;
    cout << "Select a department: ";
    cin >> deptChoice;
    clearInputBuffer();

    if (deptChoice < 1 || deptChoice > MAX_DEPARTMENTS) {
        cout << "Invalid choice. Aborting.\n";
        return;
    }

    strcpy_s(newStudent.department, departments[deptChoice - 1]);

    cout << "Available Courses:\n";
    for (int i = 0; i < MAX_COURSES; i++) {
        cout << i + 1 << ". " << courses[deptChoice - 1][i] << "\n";
    }
    int courseChoice;
    cout << "Select a course: ";
    cin >> courseChoice;
    clearInputBuffer();

    if (courseChoice < 1 || courseChoice > MAX_COURSES) {
        cout << "Invalid choice. Aborting.\n";
        return;
    }

    strcpy_s(newStudent.course, courses[deptChoice - 1][courseChoice - 1]);

    for (int sem = 0; sem < SEMESTERS; ++sem) {
        for (int subj = 0; subj < MAX_SUBJECTS; ++subj) {
            strcpy_s(newStudent.semesters[sem].subjects[subj].name, subjects[deptChoice - 1][courseChoice - 1][sem][subj]);
            newStudent.semesters[sem].subjects[subj].grade = 'N'; 
        }
    }
    newStudent.cgpa = 0;
    studentCount++;
    cout << "Student added successfully.\n";
}


bool login() {
    char username[MAX_NAME_LENGTH], password[MAX_NAME_LENGTH];
    cout << "Enter username: ";
    cin.getline(username, MAX_NAME_LENGTH);
    cout << "Enter password: ";
    cin.getline(password, MAX_NAME_LENGTH);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return true;
        }
    }
    return false;
}

void signup() {
    if (userCount >= MAX_USERS) {
        cout << "Maximum number of users reached.\n";
        return;
    }

    User& newUser = users[userCount];
    cout << "Enter new username: ";
    cin.getline(newUser.username, MAX_NAME_LENGTH);
    cout << "Enter new password: ";
    cin.getline(newUser.password, MAX_NAME_LENGTH);
    userCount++;
    saveUsers();
    cout << "Signup successful.\n";
}

void saveUsers() {
    ofstream outFile("users.dat", ios::binary);
    if (!outFile) {
        cout << "Error saving user data.\n";
        return;
    }
    outFile.write(reinterpret_cast<const char*>(&userCount), sizeof(userCount));
    outFile.write(reinterpret_cast<const char*>(users), sizeof(User) * userCount);
    outFile.close();
}

void loadUsers() {
    ifstream inFile("users.dat", ios::binary);
    if (!inFile) {
        cout << "No user data found.\n";
        return;
    }
    inFile.read(reinterpret_cast<char*>(&userCount), sizeof(userCount));
    inFile.read(reinterpret_cast<char*>(users), sizeof(User) * userCount);
    inFile.close();
}
void enterGrades() {
    char id[MAX_ID_LENGTH];
    cout << "Enter student ID: ";
    cin.getline(id, MAX_ID_LENGTH);

    for (int i = 0; i < studentCount; ++i) {
        if (strcmp(students[i].id, id) == 0) {
            for (int sem = 0; sem < SEMESTERS; ++sem) {
                cout << "Enter grades for Semester " << sem + 1 << ":\n";
                for (int subj = 0; subj < MAX_SUBJECTS; ++subj) {
                    cout << students[i].semesters[sem].subjects[subj].name << " (A/B/C/D/F): ";
                    cin >> students[i].semesters[sem].subjects[subj].grade;
                }
                calculateGPA(students[i].semesters[sem]);
            }
            calculateCGPA(students[i]);
            cout << "Grades entered successfully.\n";
            return;
        }
    }
    cout << "Student not found.\n";
}
void calculateGPA(Semester& semester) {
    float totalPoints = 0;
    int totalSubjects = 0;

    for (int i = 0; i < MAX_SUBJECTS; ++i) {
        char grade = semester.subjects[i].grade;
        float gradePoints = 0;

        switch (toupper(grade)) {
        case 'A': gradePoints = 4.0; break;
        case 'B': gradePoints = 3.0; break;
        case 'C': gradePoints = 2.0; break;
        case 'D': gradePoints = 1.0; break;
        case 'F': gradePoints = 0.0; break;
        default: gradePoints = 0.0; break;
        }

        totalPoints += gradePoints;
        totalSubjects++;
    }

    semester.gpa = (totalSubjects > 0) ? totalPoints / totalSubjects : 0.0;
}
void calculateCGPA(Student& student) {
    float totalGPA = 0;

    for (int i = 0; i < SEMESTERS; ++i) {
        totalGPA += student.semesters[i].gpa;
    }

    student.cgpa = totalGPA / SEMESTERS;
}
void generateTranscript() {
    char id[MAX_ID_LENGTH];
    cout << "Enter student ID: ";
    cin.getline(id, MAX_ID_LENGTH);

    for (int i = 0; i < studentCount; ++i) {
        if (strcmp(students[i].id, id) == 0) {
            cout << "\n--- Transcript for " << students[i].name << " (ID: " << students[i].id << ") ---\n";

            for (int sem = 0; sem < SEMESTERS; ++sem) {
                cout << "Semester " << sem + 1 << ":\n";
                cout << left << setw(30) << "Subject" << setw(10) << "Grade" << "\n";
                cout << string(40, '-') << "\n";

                for (int subj = 0; subj < MAX_SUBJECTS; ++subj) {
                    cout << left << setw(30) << students[i].semesters[sem].subjects[subj].name
                        << setw(10) << students[i].semesters[sem].subjects[subj].grade << "\n";
                }

                cout << "GPA: " << fixed << setprecision(2) << students[i].semesters[sem].gpa << "\n";
                cout << string(40, '-') << "\n";
            }

            cout << "Cumulative GPA (CGPA): " << fixed << setprecision(2) << students[i].cgpa << "\n";
            return;
        }
    }

    cout << "Student not found.\n";
}
void saveToFile() {
    ofstream outFile("students.dat", ios::binary);
    if (!outFile) {
        cout << "Error saving student data.\n";
        return;
    }

    outFile.write(reinterpret_cast<const char*>(&studentCount), sizeof(studentCount));
    outFile.write(reinterpret_cast<const char*>(students), sizeof(Student) * studentCount);
    outFile.close();
    cout << "Data saved successfully.\n";
}
void loadFromFile() {
    ifstream inFile("students.dat", ios::binary);
    if (!inFile) {
        cout << "No existing student data found.\n";
        return;
    }

    inFile.read(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
    inFile.read(reinterpret_cast<char*>(students), sizeof(Student) * studentCount);
    inFile.close();
    cout << "Data loaded successfully.\n";
}
void searchStudent() {
    char id[MAX_ID_LENGTH];
    cout << "Enter student ID to search: ";
    cin.getline(id, MAX_ID_LENGTH);

    for (int i = 0; i < studentCount; ++i) {
        if (strcmp(students[i].id, id) == 0) {
            cout << "\nStudent Found:\n";
            cout << "Name: " << students[i].name << "\n";
            cout << "ID: " << students[i].id << "\n";
            cout << "Department: " << students[i].department << "\n";
            cout << "Course: " << students[i].course << "\n";
            cout << "CGPA: " << fixed << setprecision(2) << students[i].cgpa << "\n";
            return;
        }
    }

    cout << "Student not found.\n";
}



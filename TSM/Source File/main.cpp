#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

const int maxStudents = 20;
const int maxCourses = 5;
const int maxSem = 2;
const int maxDepartments = 3;
const int maxPrograms = 3;
const int maxSubjects = 5;
const int maxUsers = 20;
const int maxNameL = 50;

struct Subject {
	string name;
	char grade;
};

struct Semester {
	Subject subjects[maxSubjects];
	float gpa;
};

struct Student {
	string id;
	string name;
	string department;
	string course;
	Semester maxSem[maxSem];
	float cgpa;
};

struct User {
	string username;
	string password;
};

Student students[maxStudents];
User  users[maxStudents]; // Assuming you want to keep the same number of users as students
int studentCount = 0;
int userCount = 0;
bool isLoggedIn = false;

// Predefined subjects for departments and courses
const char departments[maxDepartments][maxNameL] = {
	"Computer Science", "Electrical Engineering", "Mechanical Engineering"
};

const char courses[maxDepartments][maxCourses][maxNameL] = {
	{"Cybersecurity", "Software Engineering", "Data Science"},
	{"Power Systems", "Electronics", "Control Systems"},
	{"Thermodynamics", "Robotics", "Fluid Mechanics"}
};

const char subjects[maxDepartments][maxCourses][maxSem][maxSubjects][maxNameL] = {
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
	cin.ignore(10000, '\n'); // Clear the input buffer
}

void addStudent() {
	if (studentCount >= maxStudents) {
		cout << "Maximum number of students reached.\n";
		return;
	}

	Student& newStudent = students[studentCount];
	cout << "Enter student ID: ";
	getline(cin, newStudent.id);
	cout << "Enter student name: ";
	getline(cin, newStudent.name);

	// Select Department
	cout << "Available Departments:\n";
	for (int i = 0; i < maxDepartments; i++) {
		cout << i + 1 << ". " << departments[i] << "\n";
	}
	int deptChoice;
	cout << "Select a department: ";
	cin >> deptChoice;
	clearInputBuffer(); // Clear the input buffer after reading an integer

	if (deptChoice < 1 || deptChoice > maxDepartments) {
		cout << "Invalid choice. Aborting.\n";
		return;
	}

	newStudent.department = departments[deptChoice - 1];

	// Select Course
	cout << "Available Courses:\n";
	for (int i = 0; i < maxPrograms; i++) {
		cout << i + 1 << ". " << courses[deptChoice - 1][i] << "\n";
	}
	int courseChoice;
	cout << "Select a course: ";
	cin >> courseChoice;
	clearInputBuffer(); // Clear the input buffer again

	if (courseChoice < 1 || courseChoice > maxPrograms) {
		cout << "Invalid choice. Aborting.\n";
		return;
	}

	newStudent.course = courses[deptChoice - 1][courseChoice - 1];

	// Initialize subjects and grades for the maxSem
	for (int sem = 0; sem < maxSem; sem++) {
		newStudent.maxSem[sem].gpa = 0.0; // Initialize GPA
		for (int subj = 0; subj < maxSubjects; subj++) {
			newStudent.maxSem[sem].subjects[subj].name = subjects[deptChoice - 1][courseChoice - 1][sem][subj];
			newStudent.maxSem[sem].subjects[subj].grade = ' '; // Initialize grade
		}
	}

	newStudent.cgpa = 0.0; // Initialize CGPA
	studentCount++; // Increment the student count
	cout << "Student added successfully!\n";
}

bool login() {
	string username, password;
	cout << "Enter username: ";
	getline(cin, username);
	cout << "Enter password: ";
	getline(cin, password);

	for (int i = 0; i < userCount; i++) {
		if (users[i].username == username && users[i].password == password) {
			return true; // Login successful
		}
	}
	return false; // Login failed
}

// Function to sign up a new user
void signup() {
	if (userCount >= maxUsers) {
		cout << "Maximum number of users reached.\n";
		return;
	}

	User& newUser = users[userCount];
	cout << "Enter new username: ";
	getline(cin, newUser.username);
	cout << "Enter new password: ";
	getline(cin, newUser.password);
	userCount++;
	saveUsers(); // Save user data after signup
	cout << "Signup successful.\n";
}

// Function to save user data to a file
void saveUsers() {
	ofstream outFile("users.dat");
	if (!outFile) {
		cout << "Error saving user data.\n";
		return;
	}
	outFile << userCount << endl; // Save user count
	for (int i = 0; i < userCount; i++) {
		outFile << users[i].username << endl;
		outFile << users[i].password << endl;
	}
	outFile.close();
}

// Function to load user data from a file
void loadUsers() {
	ifstream inFile("users.dat");
	if (!inFile) {
		cout << "No user data found.\n";
		return;
	}
	inFile >> userCount; // Load user count
	inFile.ignore(); // Ignore the newline character after the integer
	for (int i = 0; i < userCount; i++) {
		getline(inFile, users[i].username);
		getline(inFile, users[i].password);
	}
	inFile.close();
}

void calculateGPA(Semester& semester) {
	float totalPoints = 0;
	int totalSubjects = 0;

	for (int i = 0; i < maxSubjects; ++i) {
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

	for (int i = 0; i < maxSem; ++i) {
		totalGPA += student.maxSem[i].gpa;
	}

	student.cgpa = totalGPA / maxSem;
}

void enterGrades() {
	string id;
	cout << "Enter student ID: ";
	getline(cin, id);

	for (int i = 0; i < studentCount; ++i) {
		if (students[i].id == id) {
			for (int sem = 0; sem < maxSem; ++sem) {
				cout << "Enter grades for Semester " << sem + 1 << ":\n";
				for (int subj = 0; subj < maxSubjects; ++subj) {
					cout << students[i].maxSem[sem].subjects[subj].name << " (A/B/C/D/F): ";
					cin >> students[i].maxSem[sem].subjects[subj].grade;
				}
				calculateGPA(students[i].maxSem[sem]);
			}
			calculateCGPA(students[i]);
			cout << "Grades entered successfully.\n";
			return;
		}
	}
	cout << "Student not found.\n";
}

void generateTranscript() {
	char id[20]; // Assuming a maximum length for student ID
	cout << "Enter student ID: ";
	cin.getline(id, 20);

	for (int i = 0; i < studentCount; ++i) {
		if (strcmp(students[i].id.c_str(), id) == 0) { // Use c_str() for comparison
			cout << "\n--- Transcript for " << students[i].name << " (ID: " << students[i].id << ") ---\n";

			for (int sem = 0; sem < maxSem; ++sem) {
				cout << "Semester " << sem + 1 << ":\n";
				cout << left << setw(30) << "Subject" << setw(10) << "Grade" << "\n"; // Header
				cout << string(40, '-') << "\n"; // Separator

				for (int subj = 0; subj < maxSubjects; ++subj) {
					cout << left << setw(30) << students[i].maxSem[sem].subjects[subj].name
						<< setw(10) << students[i].maxSem[sem].subjects[subj].grade << "\n"; // Subject and grade
				}

				cout << "GPA: " << fixed << setprecision(2) << students[i].maxSem[sem].gpa << "\n"; // GPA
				cout << string(40, '-') << "\n"; // Separator
			}

			cout << "Cumulative GPA (CGPA): " << fixed << setprecision(2) << students[i].cgpa << "\n"; // CGPA
			return;
		}
	}

	cout << "Student not found.\n";
}

void saveToFile() {
	ofstream outFile("students.txt");
	if (!outFile) {
		cout << "Error saving student data.\n";
		return;
	}

	outFile << studentCount << endl; // Save student count
	for (int i = 0; i < studentCount; ++i) {
		outFile << students[i].id << endl;
		outFile << students[i].name << endl;
		for (int sem = 0; sem < 2; ++sem) { // Assuming 2 maxSem
			outFile << students[i].maxSem[sem].gpa << endl; // Save GPA
			for (int subj = 0; subj < 5; ++subj) { // Assuming 5 subjects
				outFile << students[i].maxSem[sem].subjects[subj].name << endl;
				outFile << students[i].maxSem[sem].subjects[subj].grade << endl;
			}
		}
		outFile << students[i].cgpa << endl; // Save CGPA
	}

	outFile.close();
	cout << "Data saved successfully.\n";
}

void loadFromFile() {
	ifstream inFile("students.txt");
	if (!inFile) {
		cout << "No existing student data found.\n";
		return;
	}

	inFile >> studentCount; // Load student count
	inFile.ignore(); // Ignore the newline character after the integer

	for (int i = 0; i < studentCount; ++i) {
		getline(inFile, students[i].id); // Load student ID
		getline(inFile, students[i].name); // Load student name
		for (int sem = 0; sem < 2; ++sem) { // Assuming 2 maxSem
			inFile >> students[i].maxSem[sem].gpa; // Load GPA
			inFile.ignore(); // Ignore the newline character
			for (int subj = 0; subj < 5; ++subj) { // Assuming 5 subjects
				getline(inFile, students[i].maxSem[sem].subjects[subj].name); // Load subject name
				inFile >> students[i].maxSem[sem].subjects[subj].grade; // Load subject grade
				inFile.ignore(); // Ignore the newline character
			}
		}
		inFile >> students[i].cgpa; // Load CGPA
		inFile.ignore(); // Ignore the newline character
	}

	inFile.close();
	cout << "Data loaded successfully.\n";
}

void searchStudent() {
	string id;
	cout << "Enter student ID to search: ";
	getline(cin, id);

	for (int i = 0; i < studentCount; ++i) {
		if (students[i].id == id) {
			cout << "\nStudent Found:\n";
			cout << "Name: " << students[i].name << "\n";
			cout << "ID: " << students[i].id << "\n";
			cout << "Department: " << students[i].department << "\n";
			cout << "Course: " << students[i].course << "\n";
			cout << "CGPA: " << students[i].cgpa << "\n"; // No formatting
			return;
		}
	}

	cout << "Student not found.\n";
}


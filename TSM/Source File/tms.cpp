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
	{"Cyber Security", "Software Engineering", "Data Science"},
	{"Power Systems", "Electronics", "Control Systems"},
	{"Thermodynamics", "Robotics", "Fluid Mechanics"}
};

const char subjects[maxDepartments][maxCourses][maxSem][maxSubjects][maxNameL] = {
	{ // Computer Science
		{ // Cybersecurity
			{"Programming Lundamentals", "Ideology", "Functional English", "Calculus", "Applied Physics"},
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
void editStudentGrades();        // New
void deleteStudent();      // New
void backupData();         // New
void restoreData();        // New
void performanceStatistics(); // New
void viewAllStudents();

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
		displayMenu();  // Call the menu function
		cin >> choice;
		cin.ignore(); // Clear input buffer

		switch (choice) {
		case 1:
			addStudent();
			saveToFile();// Add a new student
			break;
		case 2:
			enterGrades();
			saveToFile();
			break;
		case 3:
			viewAllStudents(); // Display all students
			break;
		case 4:
			searchStudent(); // Search for a student
			break;
		case 5:
			editStudentGrades();
			saveToFile();// Edit student information
			break;
		case 6:
			deleteStudent();
			saveToFile();// Delete student record
			break;
		case 7:
			backupData(); // Backup data
			break;
		case 8:
			restoreData(); // Restore data
			break;
		case 9:
			generateTranscript(); // Generate transcript
			break;
		case 10:
			performanceStatistics(); // Show performance statistics
			break;
		case 11:
			cout << "Exiting the system. Goodbye!\n";
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
		}
	} while (choice != 11);

	return 0;
}


void displayMenu() {
	 // Clears the screen (if implemented elsewhere)
	cout << "\n--- Transcript Management System ---\n";
	cout << "1. Add Student\n";
	cout << "2. Enter Grades\n";
	cout << "3. Display All Students\n";
	cout << "4. Search for a Student\n";
	cout << "5. Edit Student Information\n"; // New
	cout << "6. Delete Student Record\n";    // New
	cout << "7. Backup Data\n";              // New
	cout << "8. Restore Data\n";             // New
	cout << "9. Generate Transcript\n";      // Updated
	cout << "10. Performance Statistics\n";   // New
	cout << "11. Exit\n";
	cout << "Enter your choice: ";
}

void clearScreen() {
#ifdef _WIN32
	system("cls"); // For Windows
#else
	system("clear"); // For Unix/Linux/MacOS
#endif
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
	cout << "\nAvailable Departments:\n";
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
	cout << "\nAvailable Courses:\n";
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
		switch (grade) {
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
			// Ask for which semester the user wants to enter grades for
			int sem;
			cout << "Enter the semester number (1 to " << maxSem << "): ";
			cin >> sem;
			sem--;  // Adjust for zero-based indexing

			if (sem >= 0 && sem < maxSem) {
				cout << "Enter grades for Semester " << sem + 1 << ":\n";

				// Loop through subjects and input grades
				for (int subj = 0; subj < maxSubjects; ++subj) {
					cout << students[i].maxSem[sem].subjects[subj].name << " (A/B/C/D/F): ";
					cin >> students[i].maxSem[sem].subjects[subj].grade;
				}

				// Recalculate GPA for the selected semester
				calculateGPA(students[i].maxSem[sem]);
				cout << "Grades entered successfully for Semester " << sem + 1 << ".\n";
			}
			else {
				cout << "Invalid semester number.\n";
			}

			// Recalculate CGPA after updating the semester grades
			calculateCGPA(students[i]);

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

			// Wait for user input before returning to the main menu
			cout << "\nPress Enter to return to the main menu...";
			cin.ignore(); // Clear any leftover newline in the buffer
			cin.get();    // Wait for Enter
			return;
		}
	}

	cout << "Student not found.\n";

	// Wait for user input before returning to the main menu
	cout << "\nPress Enter to return to the main menu...";
	cin.ignore(); // Clear any leftover newline in the buffer
	cin.get();    // Wait for Enter
}



void saveToFile() {
	ofstream outFile("students.txt", ios::ate);
	if (!outFile) {
		cout << "Error saving student data.\n";
		return;
	}

	for (int i = 0; i < studentCount; ++i) {
		outFile << "Student: " << i + 1 << endl;
		outFile << "Name: " << students[i].name << endl;
		outFile << "Reg No: " << students[i].id << endl;
		outFile << "----------------------------------------" << endl;

		// Calculate the maximum subject name length across both semesters
		int maxSubjectLength = 20; // Minimum width of 20 for consistent formatting
		for (int sem = 0; sem < 2; ++sem) { // Assuming 2 maxSem
			for (int subj = 0; subj < 5; ++subj) {
				int currentLength = students[i].maxSem[sem].subjects[subj].name.length();
				if (currentLength > maxSubjectLength) {
					maxSubjectLength = currentLength;
				}
			}
		}

		for (int sem = 0; sem < 2; ++sem) { // Loop through semesters
			outFile << "Subject Name" << string(maxSubjectLength - 11, ' ') << "Grade" << endl;
			outFile << "----------------------------------------" << endl;

			// Print subjects and grades for the semester
			for (int subj = 0; subj < 5; ++subj) {
				string subjectName = students[i].maxSem[sem].subjects[subj].name;
				char gradeChar = students[i].maxSem[sem].subjects[subj].grade;

				// Format output: Pad subject names to align grades
				outFile << subjectName << string(maxSubjectLength - subjectName.length() + 1, ' ') << gradeChar << endl;
			}

			outFile << "----------------------------------------" << endl;
			outFile << "GPA: " << students[i].maxSem[sem].gpa << endl;
			outFile << "----------------------------------------" << endl;
		}

		outFile << "----------------------------------------" << endl;
		outFile << "CGPA: " << students[i].cgpa << endl; // Save CGPA
		outFile << "----------------------------------------\n\n";
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

	studentCount = 0; // Initialize the student count
	string line;
	while (getline(inFile, line)) {
		if (line.find("Student:") != string::npos) {
			// Load student ID and name
			getline(inFile, students[studentCount].name);
			getline(inFile, students[studentCount].id);
			getline(inFile, line); // Skip the separator line

			for (int sem = 0; sem < maxSem; ++sem) {
				inFile >> students[studentCount].maxSem[sem].gpa;
				inFile.ignore(); // Skip the newline after GPA
				getline(inFile, line); // Skip the separator line

				for (int subj = 0; subj < maxSubjects; ++subj) {
					getline(inFile, students[studentCount].maxSem[sem].subjects[subj].name);
					inFile >> students[studentCount].maxSem[sem].subjects[subj].grade;
					inFile.ignore(); // Skip the newline after grade
				}

				getline(inFile, line); // Skip the separator line
			}

			inFile >> students[studentCount].cgpa;
			inFile.ignore(); // Skip the newline after CGPA
			getline(inFile, line); // Skip the separator line

			studentCount++; // Increment student count
		}
	}

	inFile.close();
	cout << "Data loaded successfully.\n";
}


void searchStudent() {
	cout << "Search by:\n1. ID\n2. Name\nEnter choice: ";
	int choice;
	cin >> choice;
	clearInputBuffer();

	if (choice == 1) {
		string id;
		cout << "Enter student ID: ";
		getline(cin, id);
		for (int i = 0; i < studentCount; ++i) {
			if (students[i].id == id) {
				// Display student details
				return;
			}
		}
	}
	else if (choice == 2) {
		string name;
		cout << "Enter student name: ";
		getline(cin, name);
		for (int i = 0; i < studentCount; ++i) {
			if (students[i].name == name) {
				// Display student details
				return;
			}
		}
	}
	cout << "Student not found.\n";
}

void viewAllStudents() {
	
	if (studentCount == 0) {
		cout << "No students available.\n";
		return;
	}

	cout << "List of all students:\n";
	for (int i = 0; i < studentCount; ++i) {
		cout << "ID: " << students[i].id
			<< ", Name: " << students[i].name
			<< ", Department: " << students[i].department
			<< ", Course: " << students[i].course
			<< ", CGPA: " << fixed << setprecision(2) << students[i].cgpa << "\n";
	}
}
void editStudentGrades() {
	string id;
	cout << "Enter student ID to edit grades: ";
	getline(cin, id);

	for (int i = 0; i < studentCount; ++i) {
		if (students[i].id == id) {
			cout << "Editing grades for student: " << students[i].name << "\n";

			// Focus only on Semester 1 (index 0)
			int sem = 0;
			cout << "Editing grades for Semester " << sem + 1 << ":\n";

			// Display all subjects in Semester 1
			for (int subj = 0; subj < maxSubjects; ++subj) {
				cout << "Current grade for " << students[i].maxSem[sem].subjects[subj].name
					<< ": " << students[i].maxSem[sem].subjects[subj].grade << "\n";
			}

			// Ask the user which subject to edit
			cout << "Enter the number of the subject to edit (1 to " << maxSubjects << "): ";
			int subjectIndex;
			cin >> subjectIndex;
			subjectIndex--;  // Adjust for zero-based indexing

			// Check if the index is valid
			if (subjectIndex >= 0 && subjectIndex < maxSubjects) {
				// Prompt for new grade
				cout << "Enter new grade for " << students[i].maxSem[sem].subjects[subjectIndex].name
					<< " (A/B/C/D/F, leave blank to keep unchanged): ";
				string newGrade;
				cin >> newGrade;

				// If new grade is entered, update it
				if (!newGrade.empty()) {
					if (newGrade.size() == 1) {  // Only one character (like 'A')
						students[i].maxSem[sem].subjects[subjectIndex].grade = newGrade[0];  // Store as char
					}
					else {
						// If grade is more than one character, store it as a string
						students[i].maxSem[sem].subjects[subjectIndex].grade = newGrade[0];  // First character
					}
				}

				// Recalculate GPA for Semester 1
				calculateGPA(students[i].maxSem[sem]);

				cout << "Grade updated successfully.\n";
			}
			else {
				cout << "Invalid subject number.\n";
			}

			// Recalculate CGPA
			calculateCGPA(students[i]);

			return;
		}
	}

	cout << "Student ID not found.\n";
}



void deleteStudent() {
	string id;
	cout << "Enter student ID to delete: ";
	getline(cin, id);

	for (int i = 0; i < studentCount; ++i) {
		if (students[i].id == id) {
			for (int j = i; j < studentCount - 1; ++j) {
				students[j] = students[j + 1]; // Shift all subsequent students up
			}
			studentCount--;
			cout << "Student deleted successfully.\n";
			return;
		}
	}
	cout << "Student not found.\n";
}

void backupData() {
	ofstream backupFile("backup.txt");
	if (!backupFile) {
		cout << "Error creating backup file.\n";
		return;
	}
	backupFile << studentCount << endl;
	for (int i = 0; i < studentCount; ++i) {
		backupFile << students[i].id << endl;
		backupFile << students[i].name << endl;
		backupFile << students[i].department << endl;
		backupFile << students[i].course << endl;
		backupFile << students[i].cgpa << endl;
		// Save other details as needed
	}
	backupFile.close();
	cout << "Backup created successfully.\n";
}

void restoreData() {
	ifstream backupFile("backup.txt");
	if (!backupFile) {
		cout << "No backup file found.\n";
		return;
	}
	backupFile >> studentCount;
	backupFile.ignore();
	for (int i = 0; i < studentCount; ++i) {
		getline(backupFile, students[i].id);
		getline(backupFile, students[i].name);
		getline(backupFile, students[i].department);
		getline(backupFile, students[i].course);
		backupFile >> students[i].cgpa;
		backupFile.ignore();
		// Load other details as needed
	}
	backupFile.close();
	cout << "Data restored successfully.\n";
}

void performanceStatistics() {
	if (studentCount == 0) {
		cout << "No students available to calculate statistics.\n";
		return;
	}

	float highestGPA = 0, lowestGPA = 4, totalGPA = 0;
	for (int i = 0; i < studentCount; ++i) {
		if (students[i].cgpa > highestGPA) highestGPA = students[i].cgpa;
		if (students[i].cgpa < lowestGPA) lowestGPA = students[i].cgpa;
		totalGPA += students[i].cgpa;
	}

	cout << "\n--- Performance Statistics ---\n";
	cout << "Highest CGPA: " << highestGPA << "\n";
	cout << "Lowest CGPA: " << lowestGPA << "\n";
	cout << "Average CGPA: " << totalGPA / studentCount << "\n";
}

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int maxStudents = 20;
const int maxCourses = 5;
const int maxSem = 2;
const int maxDepartments = 2;
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
User  users[maxStudents];
int studentCount = 0;
int userCount = 0;
bool isLoggedIn = false;

const char departments[maxDepartments][maxNameL] = {
	"Computer Science", "Electrical Engineering"
};

const char courses[maxDepartments][maxCourses][maxNameL] = {
	{"Cyber Security", "Software Engineering", "Data Science"},
	{"Power Systems", "Electronics", "Control Systems"},
};

const char subjects[maxDepartments][maxCourses][maxSem][maxSubjects][maxNameL] = {
	{ // Computer Science
		{ // Cybersecurity
			{"Programming Fundamentals", "Ideology", "Functional English", "Calculus", "Applied Physics"},
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
void loadUsers();
void saveUsers();
bool login();
void signup();
void editStudentGrades();
void deleteStudent(); 
void performanceStatistics();
void viewAllStudents();

int main() {
	int choice;

	loadUsers();
	loadFromFile();

	cout << "Welcome to the Transcript Management System\n";
	while (!isLoggedIn) {
		cout << "1. Login\n2. Signup\n3. Exit\nEnter your choice: ";
		cin >> choice;

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
		cin.ignore(); 

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
			viewAllStudents(); 
			break;
		case 4:
			searchStudent();
			break;
		case 5:
			editStudentGrades();
			saveToFile();
			break;
		case 6:
			deleteStudent();
			saveToFile();
			break;
		case 7:
			generateTranscript(); 
			break;
		case 8:
			performanceStatistics(); 
			break;
		case 9:
			cout << "Exiting the system. Goodbye!\n";
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
		}
	} while (choice != 9);

	return 0;
}


void displayMenu() {
	cout << "\n--- Transcript Management System ---\n";
	cout << "1. Add Student\n";
	cout << "2. Enter Grades\n";
	cout << "3. Display All Students\n";
	cout << "4. Search for a Student\n";
	cout << "5. Edit Student Information\n"; 
	cout << "6. Delete Student Record\n";                
	cout << "7. Generate Transcript\n";     
	cout << "8. Performance Statistics\n";   
	cout << "9. Exit\n";
	cout << "Enter your choice: ";
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

	cout << "\nAvailable Departments:\n";
	for (int i = 0; i < maxDepartments; i++) {
		cout << i + 1 << ". " << departments[i] << "\n";
	}
	int deptChoice;
	cout << "Select a department: ";
	cin >> deptChoice;

	if (deptChoice < 1 || deptChoice > maxDepartments) {
		cout << "Invalid choice. Aborting.\n";
		return;
	}

	newStudent.department = departments[deptChoice - 1];

	cout << "\nAvailable Courses:\n";
	for (int i = 0; i < maxPrograms; i++) {
		cout << i + 1 << ". " << courses[deptChoice - 1][i] << "\n";
	}
	int courseChoice;
	cout << "Select a course: ";
	cin >> courseChoice;

	if (courseChoice < 1 || courseChoice > maxPrograms) {
		cout << "Invalid choice. Aborting.\n";
		return;
	}

	newStudent.course = courses[deptChoice - 1][courseChoice - 1];

	for (int sem = 0; sem < maxSem; sem++) {
		newStudent.maxSem[sem].gpa = 0.0;
		for (int subj = 0; subj < maxSubjects; subj++) {
			newStudent.maxSem[sem].subjects[subj].name = subjects[deptChoice - 1][courseChoice - 1][sem][subj];
			newStudent.maxSem[sem].subjects[subj].grade = ' '; 
		}
	}

	newStudent.cgpa = 0.0; 
	studentCount++; 
	cout << "Student added successfully!\n";
}

bool login() {
	string username, password;
	cout << "Enter username: ";
	getline(cin, username);
	cin.ignore();
	cout << "Enter password: ";
	getline(cin, password);

	for (int i = 0; i < userCount; i++) {
		if (users[i].username == username && users[i].password == password) {
			return true; 
		}
	}
	return false; 
}

void signup() {
	if (userCount >= maxUsers) {
		cout << "Maximum number of users reached.\n";
		return;
	}

	User& newUser = users[userCount];
	cout << "Enter new username: ";
	getline(cin, newUser.username); 
	cin.sync();
	cout << "Enter new password: ";
	getline(cin, newUser.password); 
	userCount++;
	saveUsers();
	cout << "Signup successful.\n";
}


void saveUsers() {
	ofstream outFile("users.dat");
	if (!outFile) {
		cout << "Error saving user data.\n";
		return;
	}
	outFile << userCount << endl;
	for (int i = 0; i < userCount; i++) {
		outFile << users[i].username << endl;
		outFile << users[i].password << endl;
	}
	outFile.close();
}

void loadUsers() {
	ifstream inFile("users.dat");
	if (!inFile) {
		cout << "No user data found.\n";
		return;
	}
	inFile >> userCount; 
	inFile.ignore();
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
			int sem;
			cout << "Enter the semester number (1 to " << maxSem << "): ";
			cin >> sem;
			sem--; 

			if (sem >= 0 && sem < maxSem) {
				cout << "Enter grades for Semester " << sem + 1 << ":\n";

				for (int subj = 0; subj < maxSubjects; ++subj) {
					cout << students[i].maxSem[sem].subjects[subj].name << " (A/B/C/D/F): ";
					cin >> students[i].maxSem[sem].subjects[subj].grade;
				}
				calculateGPA(students[i].maxSem[sem]);
				cout << "Grades entered successfully for Semester " << sem + 1 << ".\n";
			}
			else {
				cout << "Invalid semester number.\n";
			}
			calculateCGPA(students[i]);

			return;
		}
	}

	cout << "Student not found.\n";
}

void generateTranscript() {
	char id[20];
	cout << "Enter student ID: ";
	cin.getline(id, 20);

	for (int i = 0; i < studentCount; ++i) {
		if (strcmp(students[i].id.c_str(), id) == 0) {
			cout << "\n--- Transcript for " << students[i].name << " (ID: " << students[i].id << ") ---\n";

			bool hasGrades = false;

			for (int sem = 0; sem < maxSem; ++sem) {
				bool semesterHasGrades = false;
				for (int subj = 0; subj < maxSubjects; ++subj) {
					if (students[i].maxSem[sem].subjects[subj].grade != 0){
						semesterHasGrades = true;
						break;
					}
				}

				if (semesterHasGrades) {
					hasGrades = true;
					cout << "Semester " << sem + 1 << ":\n";
					cout << "Subject                           Grade\n";
					cout << "-----------------------------------------\n";

					for (int subj = 0; subj < maxSubjects; ++subj) {
						if (students[i].maxSem[sem].subjects[subj].grade != 0){
							cout << students[i].maxSem[sem].subjects[subj].name;
							int spaces = 35 - students[i].maxSem[sem].subjects[subj].name.length();
							cout << string(spaces, ' ') << students[i].maxSem[sem].subjects[subj].grade << "\n";
						}
					}

					int gpa = students[i].maxSem[sem].gpa * 100;
					cout << "GPA: " << gpa / 100 << "." << gpa % 100 << "\n";
					cout << "-----------------------------------------\n";
				}
			}

			if (!hasGrades) {
				cout << "No grades available.\n";
			}
			int cgpa = students[i].cgpa * 100;
			cout << "Cumulative GPA (CGPA): " << cgpa / 100 << "." << cgpa % 100 << "\n";

			cout << "\nPress Enter to return to the main menu...";
			cin.ignore();
			cin.get();
			return;
		}
	}

	cout << "Student not found.\n";
	cout << "\nPress Enter to return to the main menu...";
	cin.ignore();
	cin.get();
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

		int maxSubjectLength = 20; 
		for (int sem = 0; sem < 2; ++sem) { 
			for (int subj = 0; subj < 5; ++subj) {
				int currentLength = students[i].maxSem[sem].subjects[subj].name.length();
				if (currentLength > maxSubjectLength) {
					maxSubjectLength = currentLength;
				}
			}
		}

		for (int sem = 0; sem < 2; ++sem) { 
			outFile << "Subject Name" << string(maxSubjectLength - 11, ' ') << "Grade" << endl;
			outFile << "----------------------------------------" << endl;

			for (int subj = 0; subj < 5; ++subj) {
				string subjectName = students[i].maxSem[sem].subjects[subj].name;
				char gradeChar = students[i].maxSem[sem].subjects[subj].grade;
				outFile << subjectName << string(maxSubjectLength - subjectName.length() + 1, ' ') << gradeChar << endl;
			}

			outFile << "----------------------------------------" << endl;
			outFile << "GPA: " << students[i].maxSem[sem].gpa << endl;
			outFile << "----------------------------------------" << endl;
		}

		outFile << "----------------------------------------" << endl;
		outFile << "CGPA: " << students[i].cgpa << endl; 
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

	studentCount = 0; 
	string line;
	while (getline(inFile, line)) {
		if (line.find("Student:") != string::npos) {
			getline(inFile, students[studentCount].name);
			getline(inFile, students[studentCount].id);
			getline(inFile, line); 

			for (int sem = 0; sem < maxSem; ++sem) {
				inFile >> students[studentCount].maxSem[sem].gpa;
				inFile.ignore(); 
				getline(inFile, line); 

				for (int subj = 0; subj < maxSubjects; ++subj) {
					getline(inFile, students[studentCount].maxSem[sem].subjects[subj].name);
					inFile >> students[studentCount].maxSem[sem].subjects[subj].grade;
					inFile.ignore(); 
				}
				getline(inFile, line); 
			}

			inFile >> students[studentCount].cgpa;
			inFile.ignore(); 
			getline(inFile, line);

			studentCount++;
		}
	}

	inFile.close();
	cout << "Data loaded successfully.\n";
}

void searchStudent() {
	cout << "Search by:\n1. ID\n2. Name\nEnter choice: ";
	int choice;
	cin >> choice;


	if (choice == 1) {
		string id;
		cout << "Enter student ID: ";
		getline(cin, id);
		for (int i = 0; i < studentCount; ++i) {
			if (students[i].id == id) {
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
		int cgpa = students[i].cgpa * 100;

		cout << "ID: " << students[i].id;
		cout << ", Name: " << students[i].name;
		cout << ", Department: " << students[i].department;
		cout << ", Course: " << students[i].course;
		cout << ", CGPA: " << cgpa / 100 << "." << cgpa % 100 << "\n";
	}
}

void editStudentGrades() {
	string id;
	cout << "Enter student ID to edit grades: ";
	getline(cin, id);

	for (int i = 0; i < studentCount; ++i) {
		if (students[i].id == id) {
			cout << "Editing grades for student: " << students[i].name << "\n";

			int sem = 0;
			cout << "Editing grades for Semester " << sem + 1 << ":\n";

			for (int subj = 0; subj < maxSubjects; ++subj) {
				cout << "Current grade for " << students[i].maxSem[sem].subjects[subj].name
					<< ": " << students[i].maxSem[sem].subjects[subj].grade << "\n";
			}

			cout << "Enter the number of the subject to edit (1 to " << maxSubjects << "): ";
			int subjectIndex;
			cin >> subjectIndex;
			subjectIndex--; 

			if (subjectIndex >= 0 && subjectIndex < maxSubjects) {
				cout << "Enter new grade for " << students[i].maxSem[sem].subjects[subjectIndex].name
					<< " (A/B/C/D/F, leave blank to keep unchanged): ";
				string newGrade;
				cin >> newGrade;

				if (!newGrade.empty()) {
					if (newGrade.size() == 1) { 
						students[i].maxSem[sem].subjects[subjectIndex].grade = newGrade[0];  
					}
					else {
						students[i].maxSem[sem].subjects[subjectIndex].grade = newGrade[0];
					}
				}
				calculateGPA(students[i].maxSem[sem]);
				cout << "Grade updated successfully.\n";
			}
			else {
				cout << "Invalid subject number.\n";
			}
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
				students[j] = students[j + 1]; 
			}
			studentCount--;
			cout << "Student deleted successfully.\n";
			return;
		}
	}
	cout << "Student not found.\n";
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
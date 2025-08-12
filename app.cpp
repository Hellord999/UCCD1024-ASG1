#define _CRT_SECURE_NO_WARNINGS
#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"


using namespace std;

bool ReadFile(string, List *);
bool DeleteRecord(List *, char *);
bool Display(List, int, int);
bool InsertBook(string, List *);
bool SearchStudent(List *, char *id, LibStudent &);
bool computeAndDisplayStatistics(List *);
bool printStuWithSameBook(List *, char *);
bool displayWarnedStudent(List *, List *, List *);
int menu();

//ADDITIONAL FUNCTIONS
bool CheckStudentExists(List*, const char*);
List* studentList = new List;

int main() {

	int choice;
	do {
		choice = menu();
		switch (choice) {
        case -1: {
            cout << "\nInvalid input. Please enter a number from 1 to 9.\n\n";
            continue;
        }
        case 1: {
            string filename;
            cout << "Please enter filename: ";
            cin >> filename;
            if (ReadFile(filename, studentList)) {
                cout << "Read File Successful\n\n";
            }
            else {
                cout << "Read File Failed\n\n";
            }
            break;
            
        }
        case 2: {
            char id[20];
            cout << "Please enter student ID: ";
            cin >> id;
            if (DeleteRecord(studentList, id)) {
                cout << "Record Deleted\n\n";
            }
            break;
        }
        case 3: {
            char id[20];
            LibStudent stu;
            cout << "Please enter student ID: ";
            cin >> id;
            if (SearchStudent(studentList, id, stu)) {
                stu.print(cout);//display student
                cout << endl;
            }
            break;
        }
        case 4: {

        }
        case 5: {

        }
        case 6: {

        }
        case 7: {

        }
        case 8: {

        }

        }

      
	} while (choice != 9);


	cout << "\n\n";
	system("pause");
	return 0;
}

int menu() {
	int choice;
	cout << "1. Read File\n";
    cout << "2. Delete record\n";
    cout << "3. Search student\n";
    cout << "4. Insert book\n";
    cout << "5. Display output\n";
    cout << "6. Compute and Display Statistics\n";
    cout << "7. Student with Same Book\n";
    cout << "8. Display Warned Student\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;
    if (cin.fail()) {//prevent invalid input
        cin.clear(); 
        cin.ignore(10000, '\n'); 
        return -1;
    }
    if (choice < 1 || choice > 9) {
        return -1;
    }
    return choice;
}

bool CheckStudentExists(List* list, const char* id) {//check duplicate record
    for (int i = 1; i <= list->size(); ++i) {
        LibStudent student;
        list->get(i, student);
        if (strcmp(student.id, id) == 0) {
            return true;
        }
    }
    return false;
}

bool ReadFile(string filename, List* list) {
    cout << "\n\nREAD FILE" << endl;
    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "File cannot be opened!" << endl;
        return false;
    }

    string line;
    int count = 0;
    LibStudent student;
    bool ok = false;

    while (getline(infile, line)) {
        if (line.empty() || line.find("Student Id =") == string::npos) {// skip empty line/no id line
            continue;
        }

        string id_str = line.substr(line.find("=") + 2); //extract id

        if (CheckStudentExists(list, id_str.c_str())) {
            cout << "Duplicate record found for Student ID: " << id_str << endl;//skip duplicate record details
            for (int i = 0; i < 3; ++i) {
                getline(infile, line);
            }
            continue;
        }

        //details to student struct
        strcpy(student.id, id_str.c_str());

        getline(infile, line);
        strcpy(student.name, line.substr(line.find("=") + 2).c_str());

        getline(infile, line);
        strcpy(student.course, line.substr(line.find("=") + 2).c_str());

        getline(infile, line);
        strcpy(student.phone_no, line.substr(line.find("=") + 2).c_str());

        list->insert(student); //add full record to list
        count++;
        ok = true;//verify that at least 1 record read
    }

    infile.close();
    cout << "\n\n" << count << " records have been successfully read.\n\n" << endl;
    return ok;
}

bool DeleteRecord(List* list, char* id) {
    if (list == NULL || list->size() == 0) {//check for list
        cout << "\nNo list found, please read file first\n\n";
        return false;
    }
    for (int i = 1; i <= list->size(); ++i) {
        LibStudent student;
        list->get(i, student);//find student
        if (strcmp(student.id, id) == 0) {//if found
            list->remove(i);
            return true;
        }
    }
    cout << "No match for " << id << " found\n\n";
    return false;
}

bool SearchStudent(List* list, char* id, LibStudent& stu) {
    if (list == NULL || list->size() == 0) {
        cout << "\nNo list found, please read file first\n\n";
        return false;
    }
    for (int i = 1; i <= list->size(); ++i) {
        LibStudent student;
        list->get(i, student);
        if (strcmp(student.id, id) == 0) {
            stu = student;
            return true;
        }
    }
    cout << "No match for " << id << " found\n\n";
    return false;
}



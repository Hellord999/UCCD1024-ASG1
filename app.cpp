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
bool Display(List *, int, int);
bool InsertBook(string, List *);
bool SearchStudent(List *, char *id, LibStudent &);
bool computeAndDisplayStatistics(List *);
bool printStuWithSameBook(List *, char *);
bool displayWarnedStudent(List *, List *, List *);
int menu();

//ADDITIONAL FUNCTIONS
bool CheckStudentExists(List*, const char*);
List* studentList = new List;

void replaceUnderscores(char*);

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
            string filename;
            cout << "Please enter filename: ";
            cin >> filename;
            if (InsertBook(filename, studentList)) {
                cout << "Books inserted successfully.\n\n";
            }
            else {
                cout << "Failed to insert books.\n\n";
            }
            break;
        }
        case 5: {
            int source, detail;
            cout << "Enter source (1 for file, 2 for screen): ";
            cin >> source;
            cout << "Enter detail level (1 for student and book, 2 for student only): ";
            cin >> detail;
            if (Display(studentList, source, detail)) {
                cout << "Display successful.\n\n";
            }
            else {
                cout << "Failed to display.\n\n";
            }
            break;
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

void replaceUnderscores(char* s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '_') {
            s[i] = ' ';
        }
    }
}

bool InsertBook(string filename, List* list) {
    ifstream inFile(filename.c_str());
    if (!inFile.is_open()) {
        cout << "[ERROR] Cannot open file: " << filename << endl;
        return false;
    }

    char stu_id[20];
    while (inFile >> stu_id) {
        LibBook book;
        char authors_str[300], title_str[250], publisher_str[30], ISBN_str[11], callNum_str[20];
        int yearPublished;
        char borrowDateStr[15], dueDateStr[15];

        inFile >> authors_str;
        inFile >> title_str;
        inFile >> publisher_str;
        inFile >> ISBN_str;
        inFile >> yearPublished;
        inFile >> callNum_str;
        inFile >> borrowDateStr;
        inFile >> dueDateStr;

        replaceUnderscores(title_str);
        replaceUnderscores(publisher_str);

        strcpy(book.title, title_str);
        strcpy(book.publisher, publisher_str);
        strcpy(book.ISBN, ISBN_str);
        book.yearPublished = yearPublished;
        strcpy(book.callNum, callNum_str);

        int authorIndex = 0;
        char authors_copy[300];
        strcpy(authors_copy, authors_str); // Make a copy for strtok
        char* token = strtok(authors_copy, "/");
        while (token != NULL && authorIndex < 10) {
            replaceUnderscores(token);
            book.author[authorIndex] = new char[strlen(token) + 1];
            strcpy(book.author[authorIndex], token);
            authorIndex++;
            token = strtok(NULL, "/");
        }
        for (int i = authorIndex; i < 10; i++) {
            book.author[i] = NULL;
        }

        sscanf(borrowDateStr, "%d/%d/%d", &book.borrow.day, &book.borrow.month, &book.borrow.year);
        sscanf(dueDateStr, "%d/%d/%d", &book.due.day, &book.due.month, &book.due.year);

        LibStudent* stu = NULL;
        Node* cur = list->head;
        while (cur != NULL) {
            if (strcmp(cur->item.id, stu_id) == 0) {
                stu = &cur->item;
                break;
            }
            cur = cur->next;
        }

        if (stu == NULL) {
            cout << "[WARNING] Student with ID " << stu_id << " not found. Skipping this book.\n";
            continue;
        }

        int toJulian = book.due.year * 360 + book.due.month * 30 + book.due.day;
        int currentJulian = 2020 * 360 + 3 * 30 + 29;
        int overdueDays = currentJulian - toJulian;
        if (overdueDays > 0) {
            book.fine = overdueDays * 0.50;
        }
        else {
            book.fine = 0.0;
        }

        if (stu->totalbook < 15) {
            LibBook* newBook = new LibBook;
            *newBook = book;
            stu->book[stu->totalbook] = *newBook;
            stu->totalbook++;
            stu->calculateTotalFine();
        }
        else {
            cout << "[WARNING] Student " << stu_id << " already has maximum number of books.\n";
        }
    }

    inFile.close();
    return true;
}

bool Display(List* list, int source, int detail) {
    if (list->empty()) {
        cout << "Error: List is empty.\n";
        return false;
    }

    Node* cur = list->head;
    int studentNum = 1;

    if (source == 1) {
        string filename = (detail == 1) ? "student_booklist.txt" : "student_info.txt";
        ofstream outFile(filename.c_str());
        if (!outFile.is_open()) {
            cout << "Error: Cannot open output file.\n";
            return false;
        }

        while (cur != NULL) {
            outFile << "\nSTUDENT " << studentNum++ << "\n\n";
            cur->item.print(outFile);

            if (detail == 1) {
                outFile << "\nBOOK LIST:\n";
                for (int i = 0; i < cur->item.totalbook; i++) {
                    outFile << "\nBook " << (i + 1) << "\n";
                    cur->item.book[i].print(outFile);
                }
            }

            outFile << "\n*****************************************************************************\n";
            cur = cur->next;
        }

        outFile.close();
    }
    else {
        while (cur != NULL) {
            cout << "\nSTUDENT " << studentNum++ << "\n\n";
            cur->item.print(cout);

            if (detail == 1) {
                cout << "\nBOOK LIST:\n";
                for (int i = 0; i < cur->item.totalbook; i++) {
                    cout << "\nBook " << (i + 1) << "\n";
                    cur->item.book[i].print(cout);
                }
            }

            cout << "\n*****************************************************************************\n";
            cur = cur->next;
        }
    }

    return true;
}



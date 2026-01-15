#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
using namespace std;

struct Student;
struct Course;
struct StudentNode;
struct CourseNode;

// Notification for Stack
struct Notification {
    string message;
    time_t timestamp;
    Notification* next;
    
    Notification(string msg) : message(msg), timestamp(time(0)), next(NULL) {}
};

// Stack for Notifications
class NotificationStack {
private:
    Notification* top;
    
public:
    NotificationStack() : top(NULL) {}
    
    void push(string message) {
        Notification* newNotif = new Notification(message);
        newNotif->next = top;
        top = newNotif;
    }
    
    void pop() {
        if (top == NULL) return;
        Notification* temp = top;
        top = top->next;
        delete temp;
    }
    
    Notification* getTop() {
        return top;
    }
    
    bool isEmpty() {
        return top == NULL;
    }
    
    void clear() {
        while (top != NULL) {
            pop();
        }
    }

    ~NotificationStack() {
        clear();
    }
};

// Student structure
struct Student {
    string name;
    string matricNo;
    int maxCredit;
    int currentCredit;
    int year;
    string program;
    CourseNode* courseList;
    NotificationStack notifications;
    Student* left;
    Student* right;

    Student(string _name, string _matricNo, int _maxCredit, int _currentCredit, int _year, string _program) 
    : name(_name), matricNo(_matricNo), maxCredit(_maxCredit), currentCredit(_currentCredit), year(_year), program(_program), courseList(NULL), notifications(), left(NULL), right(NULL) {}
};

// Course structure
struct Course {
    string courseCode;
    string courseName;
    string lecturerName;
    int credit;
    int capacity;
    int currentCapacity;
    StudentNode* studentList;
    Course* left;
    Course* right;

    Course(string _courseCode, string _courseName, string _lecturerName, int _credit, int _capacity, int _currentCapacity) 
    : courseCode(_courseCode), courseName(_courseName), lecturerName(_lecturerName), credit(_credit), capacity(_capacity), currentCapacity(_currentCapacity), studentList(NULL), left(NULL), right(NULL) {}
};

// Student Node in Linked List (for course's student list)
struct StudentNode {
    string matricNo;
    string name;
    StudentNode* next;
    
    StudentNode(string m, string n) : matricNo(m), name(n), next(NULL) {}
};

// Course Node in Linked List (for student's course list)
struct CourseNode {
    string courseCode;
    string status; // "Pending" or "Approved"
    CourseNode* next;
    
    CourseNode(string code, string stat) : courseCode(code), status(stat), next(NULL) {}
};

// Registration Request for Queue
struct RegistrationRequest {
    string matricNo;
    string studentName;
    string courseCode;
    string courseName;
    int credit;
    time_t timestamp;
    bool isCancelled;
    RegistrationRequest* next;
    
    RegistrationRequest(string matric, string sName, string cCode, string cName, int cred) 
    : matricNo(matric), studentName(sName), courseCode(cCode), courseName(cName),
      credit(cred), timestamp(time(0)), isCancelled(false), next(NULL) {}
};

// Queue for Registration Requests
class RegistrationQueue {
private:
    RegistrationRequest* front;
    RegistrationRequest* rear;
    
public:
    RegistrationQueue() : front(NULL), rear(NULL) {}

    void enqueue(string matric, string sName, string code, string cName, int cred) {
        RegistrationRequest* newReq = new RegistrationRequest(matric, sName, code, cName, cred);
        if (rear == NULL) {
            front = rear = newReq;
        } else {
            rear->next = newReq;
            rear = newReq;
        }
    }
    
    RegistrationRequest* dequeue() {
        if (front == NULL) return NULL;
        RegistrationRequest* temp = front;
        front = front->next;
        if (front == NULL) rear = NULL;
        return temp;
    }
    
    RegistrationRequest* getFront() {
        return front;
    }

    RegistrationRequest* getRear() {
        return rear;
    }
    
    bool isEmpty() {
        return front == NULL;
    }
    
    void display() {
        if (front == NULL) {
            cout << "No pending requests.\n";
            return;
        }
        
        RegistrationRequest* temp = front;
        int count = 1;
        while (temp != NULL) {
            if (!temp->isCancelled) {
                cout << count++ << ". " << temp->studentName << " (" << temp->matricNo 
                     << ") - " << temp->courseCode << " " << temp->courseName << "\n";
            }
            temp = temp->next;
        }
    }

    ~RegistrationQueue() {
        while (!isEmpty()) {
            RegistrationRequest* temp = dequeue();
            delete temp;
        }
    }
};

// Course BST
class CourseBST {
private:
    Course* root;
    
    Course* insertRec(Course* node, Course* newCourse) {
        if (node == NULL) return newCourse;
        
        if (newCourse->courseCode < node->courseCode)
            node->left = insertRec(node->left, newCourse);
        else if (newCourse->courseCode > node->courseCode)
            node->right = insertRec(node->right, newCourse);
            
        return node;
    }
    
    Course* searchRec(Course* node, string code) {
        if (node == NULL || node->courseCode == code)
            return node;
            
        if (code < node->courseCode)
            return searchRec(node->left, code);
        return searchRec(node->right, code);
    }
    
    void inorderRec(Course* node) {
        if (node != NULL) {
            inorderRec(node->left);
            cout << node->courseCode << " | " << node->courseName << " | " 
                 << node->credit << " credit(s) | " << node->currentCapacity 
                 << "/" << node->capacity << " students\n";
            inorderRec(node->right);
        }
    }
    
    Course* deleteRec(Course* node, string code) {
        if (!node) return nullptr;

        if (code < node->courseCode) {
            node->left = deleteRec(node->left, code);
        }
        else if (code > node->courseCode) {
            node->right = deleteRec(node->right, code);
        }
        else {
            // Case 1: no left child
            if (!node->left) {
                Course* temp = node->right;
                node->left = node->right = nullptr;
                delete node;
                return temp;
            }
            // Case 2: no right child
            else if (!node->right) {
                Course* temp = node->left;
                node->left = node->right = nullptr;
                delete node;
                return temp;
            }

            // Case 3: two children
            Course* successor = minValueNode(node->right);
            node->courseCode = successor->courseCode;
            node->courseName = successor->courseName;
            node->lecturerName = successor->lecturerName;
            node->credit = successor->credit;
            node->capacity = successor->capacity;
            node->currentCapacity = successor->currentCapacity;
            node->studentList = successor->studentList;
            successor->studentList = nullptr;
            node->right = deleteRec(node->right, successor->courseCode);
        }
        return node;
    }

    
    Course* minValueNode(Course* node) {
        Course* current = node;
        while (current && current->left != NULL)
            current = current->left;
        return current;
    }

    void destroyCourseTree(Course* node) {
    if (!node) return;

    destroyCourseTree(node->left);
    destroyCourseTree(node->right);

    StudentNode* cur = node->studentList;
    while (cur) {
        StudentNode* temp = cur;
        cur = cur->next;
        delete temp;
    }

    delete node;
}
    
public:
    CourseBST() : root(NULL) {}
    
    void insert(Course* newCourse) {
        root = insertRec(root, newCourse);
    }
    
    Course* search(string code) {
        return searchRec(root, code);
    }
    
    void displayAll() {
        if (root == NULL) {
            cout << "No courses available.\n";
            return;
        }
        cout << "\n=== All Courses ===\n";
        inorderRec(root);
    }
    
    void deleteCourse(string code) {
        root = deleteRec(root, code);
    }
    
    Course* getRoot() { return root; }
    
    ~CourseBST() {
        destroyCourseTree(root);
        root = NULL;
    }
};

// Student BST
class StudentBST {
    private:
        Student* root;
        
        Student* insertRec(Student* node, Student* newStudent) {
            if (node == NULL) return newStudent;
            
            if (newStudent->matricNo < node->matricNo)
                node->left = insertRec(node->left, newStudent);
            else if (newStudent->matricNo > node->matricNo)
                node->right = insertRec(node->right, newStudent);
                
            return node;
        }
        
        Student* searchRec(Student* node, string matric) {
            if (node == NULL || node->matricNo == matric)
                return node;
                
            if (matric < node->matricNo)
                return searchRec(node->left, matric);
            return searchRec(node->right, matric);
        }
        
        void inorderRec(Student* node) {
            if (node != NULL) {
                inorderRec(node->left);
                cout << node->name << " | " << node->matricNo << " | Year " 
                    << node->year << " | " << node->program << " | " 
                    << node->currentCredit << "/" << node->maxCredit << " credits\n";
                inorderRec(node->right);
            }
        }
        
        Student* deleteRec(Student* node, string matric) {
            if (!node) return nullptr;

            if (matric < node->matricNo) {
                node->left = deleteRec(node->left, matric);
            }
            else if (matric > node->matricNo) {
                node->right = deleteRec(node->right, matric);
            }
            else {
                // Case 1: no left child
                if (!node->left) {
                    Student* temp = node->right;
                    node->left = node->right = nullptr;
                    delete node;
                    return temp;
                }
                // Case 2: no right child
                else if (!node->right) {
                    Student* temp = node->left;
                    node->left = node->right = nullptr;
                    delete node;
                    return temp;
                }

                // Case 3: two children
                Student* successor = minValueNode(node->right);
                node->name = successor->name;
                node->matricNo = successor->matricNo;
                node->maxCredit = successor->maxCredit;
                node->currentCredit = successor->currentCredit;
                node->year = successor->year;
                node->program = successor->program;
                node->courseList = successor->courseList;
                node->notifications = successor->notifications;
                successor->courseList = nullptr;
                successor->notifications.clear();
                node->right = deleteRec(node->right, successor->matricNo);
            }
            return node;
        }

        
        Student* minValueNode(Student* node) {
            Student* current = node;
            while (current && current->left != NULL)
                current = current->left;
            return current;
        }

        void destroyStudentTree(Student* node) {
        if (!node) return;

        destroyStudentTree(node->left);
        destroyStudentTree(node->right);

        CourseNode* cur = node->courseList;
        while (cur) {
            CourseNode* temp = cur;
            cur = cur->next;
            delete temp;
        }

        node->notifications.clear();

        delete node;
    }
        
    public:
        StudentBST() : root(NULL) {}
        
        void insert(Student* newStudent) {
            root = insertRec(root, newStudent);
        }
        
        Student* search(string matric) {
            return searchRec(root, matric);
        }
        
        void displayAll() {
            if (root == NULL) {
                cout << "No students in system.\n";
                return;
            }
            cout << "\n=== All Students ===\n";
            inorderRec(root);
        }
        
        Student* getRoot() { return root; }

        ~StudentBST() {
            destroyStudentTree(root);
            root = NULL;
        }
};

// Staff structure
struct Staff {
    string name;
    string staffID;
};

// Global variables
StudentBST studentTree;
CourseBST courseTree;
vector<Staff> staffList;
RegistrationQueue regQueue;

// Function prototypes
void loadStudents();
void loadCourses();
void loadStaff();
void loadPendingRequests();
void saveStudents();
void saveCourses();
void savePendingRequests();
void saveAllData();
void saveStudentRec(Student* node, ofstream& file);
void saveCourseRec(Course* node, ofstream& file);
void studentMenu(Student* student);
void staffMenu(Staff* staff);
void viewAllCourses();
void searchCourse();
void registerCourse(Student* student);
void viewRegisteredCourses(Student* student);
void deleteCourse(Student* student);
void viewNotifications(Student* student);
void viewRegistrationSlip(Student* student);
void viewStudentsInCourse(Student* student);
void staffViewRequests();
void staffViewAllStudents();
void staffSearchStudent();
void staffInsertStudent();
void staffUpdateCourse();
void staffInsertCourse();
void staffDeleteCourse();
void staffViewStudentListInCourse();
void staffViewAllCourses();
void staffRetractCourse();
void staffInsertCourseForStudent();
void insertStudentInCourseList(Course* course, string matric, string name);
void removeStudentFromCourseList(Course* course, string matric);
void markRequestAsCancelled(string matric, string courseCode);
void addCourseToStudent(Student* student, string courseCode, string status);
void removeCourseFromStudent(Student* student, string courseCode);
CourseNode* findCourseInStudent(Student* student, string courseCode);
int countStudentCourses(Student* student);
string getCurrentTimestamp(time_t t = 0);
void easterEgg();

void addCourseToStudent(Student* student, string courseCode, string status) {
    CourseNode* newNode = new CourseNode(courseCode, status);
    newNode->next = student->courseList;
    student->courseList = newNode;
}

void removeCourseFromStudent(Student* student, string courseCode) {
    if (student->courseList == NULL) return;
    
    if (student->courseList->courseCode == courseCode) {
        CourseNode* temp = student->courseList;
        student->courseList = student->courseList->next;
        delete temp;
        return;
    }
    
    CourseNode* current = student->courseList;
    while (current->next != NULL && current->next->courseCode != courseCode) {
        current = current->next;
    }
    
    if (current->next != NULL) {
        CourseNode* temp = current->next;
        current->next = current->next->next;
        delete temp;
    }
}

CourseNode* findCourseInStudent(Student* student, string courseCode) {
    CourseNode* current = student->courseList;
    while (current != NULL) {
        if (current->courseCode == courseCode) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int countStudentCourses(Student* student) {
    int count = 0;
    CourseNode* current = student->courseList;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

int main() {
    loadStudents();
    loadCourses();
    loadStaff();
    loadPendingRequests();
    
    while (true) {
        cout << "\n========================================\n";
        cout << "   COURSE REGISTRATION SYSTEM\n";
        cout << "========================================\n";
        cout << "1. Student\n";
        cout << "2. Staff\n";
        cout << "3. Exit\n";
        cout << "Select role: ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        easterEgg();
        if (choice == 1) {
            cout << "Enter Matric No: ";
            string matric;
            getline(cin, matric);
            
            Student* student = studentTree.search(matric);
            if (student == NULL) {
                cout << "You are not eligible for course registration.\n";
            } else {
                studentMenu(student);
            }
        } else if (choice == 2) {
            cout << "Enter Staff ID: ";
            string staffID;
            getline(cin, staffID);
            
            Staff* foundStaff = NULL;
            for (int i = 0; i < staffList.size(); i++) {
                if (staffList[i].staffID == staffID) {
                    foundStaff = &staffList[i];
                    break;
                }
            }
            
            if (foundStaff == NULL) {
                cout << "Prohibited access.\n";
            } else {
                cout << "Welcome " << foundStaff->name << "!\n";
                staffMenu(foundStaff);
            }
        } else if (choice == 3) {
            saveAllData();
            cout << "Thank you for using the system!\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
    
    return 0;
}

void loadStudents() {
    ifstream file("students.txt");
    if (!file) {
        cout << "Students file not found. Starting with empty student list.\n";
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        int pos = 0;
        int tabPos = line.find('\t');
        
        string studentName;
        studentName = line.substr(pos, tabPos - pos);
        pos = tabPos + 1;
        
        string matricNo;
        tabPos = line.find('\t', pos);
        matricNo = line.substr(pos, tabPos - pos);
        pos = tabPos + 1;
        
        int maxCredit;
        tabPos = line.find('\t', pos);
        maxCredit = atoi(line.substr(pos, tabPos - pos).c_str());
        pos = tabPos + 1;
        
        int currentCredit;
        tabPos = line.find('\t', pos);
        currentCredit = atoi(line.substr(pos, tabPos - pos).c_str());
        pos = tabPos + 1;
        
        int year;
        tabPos = line.find('\t', pos);
        year = atoi(line.substr(pos, tabPos - pos).c_str());
        pos = tabPos + 1;
        
        string program;
        tabPos = line.find('\t', pos);
        program = line.substr(pos, tabPos - pos);
        pos = tabPos + 1;
        Student* newStudent = new Student(studentName, matricNo, maxCredit, currentCredit, year, program);
        // Number of courses
        int numCourses = atoi(line.substr(pos).c_str());
        
        // Read registered courses from following lines
        for (int i = 0; i < numCourses; i++) {
            if (getline(file, line)) {
                // Parse: CourseCode\tStatus
                tabPos = line.find('\t');
                if (tabPos != string::npos) {
                    string courseCode = line.substr(0, tabPos);
                    string status = line.substr(tabPos + 1);
                    addCourseToStudent(newStudent, courseCode, status);
                }
            }
        }
        
        studentTree.insert(newStudent);
    }
    
    file.close();
    cout << "Students loaded successfully.\n";
}

void loadCourses() {
    ifstream file("courses.txt");
    if (!file) {
        cout << "Courses file not found. Starting with empty course list.\n";
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        string courseCode, courseName, lecturerName;
        int credit, capacity, currentCapacity;
        
        int pos = 0;
        int tabPos = line.find('\t');
        
        // CourseCode
        courseCode = line.substr(pos, tabPos - pos);
        pos = tabPos + 1;
        
        // CourseName
        tabPos = line.find('\t', pos);
        courseName = line.substr(pos, tabPos - pos);
        pos = tabPos + 1;
        
        // LecturerName
        tabPos = line.find('\t', pos);
        lecturerName = line.substr(pos, tabPos - pos);
        pos = tabPos + 1;
        
        // Credit
        tabPos = line.find('\t', pos);
        credit = atoi(line.substr(pos, tabPos - pos).c_str());
        pos = tabPos + 1;
        
        // Capacity
        tabPos = line.find('\t', pos);
        capacity = atoi(line.substr(pos, tabPos - pos).c_str());
        pos = tabPos + 1;
        
        // CurrentCapacity
        tabPos = line.find('\t', pos);
        currentCapacity = atoi(line.substr(pos, tabPos - pos).c_str());
        pos = tabPos + 1;
        // Number of students
        int numStudents = atoi(line.substr(pos).c_str());
        Course* newCourse = new Course(courseCode, courseName, lecturerName, credit, capacity, currentCapacity);
        
        // Read students from following lines
        for (int i = 0; i < numStudents; i++) {
            if (getline(file, line)) {
                // Parse: MatricNo\tName
                tabPos = line.find('\t');
                if (tabPos != string::npos) {
                    string matric = line.substr(0, tabPos);
                    string name = line.substr(tabPos + 1);
                    insertStudentInCourseList(newCourse, matric, name);
                }
            }
        }
        
        courseTree.insert(newCourse);
    }
    
    file.close();
    cout << "Courses loaded successfully.\n";
}

void loadStaff() {
    ifstream file("staff.txt");
    if (!file) {
        cout << "Staff file not found.\n";
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        Staff newStaff;
        
        // Parse tab-separated values
        int tabPos = line.find('\t');
        newStaff.name = line.substr(0, tabPos);
        newStaff.staffID = line.substr(tabPos + 1);
        
        staffList.push_back(newStaff);
    }
    
    file.close();
    cout << "Staff loaded successfully.\n";
}

void loadPendingRequests() {
    ifstream file("pending_requests.txt");
    if (!file) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        string matric, sName, cCode, cName;
        int credit;
        time_t timestamp;

        int pos = 0, tab;
        tab = line.find('\t');
        matric = line.substr(pos, tab - pos);
        pos = tab + 1;
        tab = line.find('\t', pos);
        sName = line.substr(pos, tab - pos);
        pos = tab + 1;
        tab = line.find('\t', pos);
        cCode = line.substr(pos, tab - pos);
        pos = tab + 1;
        tab = line.find('\t', pos);
        cName = line.substr(pos, tab - pos);
        pos = tab + 1;
        tab = line.find('\t', pos);
        credit = atoi(line.substr(pos, tab - pos).c_str());
        pos = tab + 1;
        timestamp = atol(line.substr(pos).c_str());
        regQueue.enqueue(matric, sName, cCode, cName, credit);
        regQueue.getRear()->timestamp = timestamp;
    }

    file.close();
}

void saveStudentRec(Student* node, ofstream& file) {
    if (node == NULL) return;
    
    saveStudentRec(node->left, file);
    
    file << node->name << "\t";
    file << node->matricNo << "\t";
    file << node->maxCredit << "\t";
    file << node->currentCredit << "\t";
    file << node->year << "\t";
    file << node->program << "\t";
    file << countStudentCourses(node);
    
    CourseNode* current = node->courseList;
    while (current != NULL) {
        file << "\n" << current->courseCode << "\t" << current->status;
        current = current->next;
    }
    file << "\n";
    
    saveStudentRec(node->right, file);
}

void saveCourseRec(Course* node, ofstream& file) {
    if (node == NULL) return;
    
    saveCourseRec(node->left, file);
    
    // Count students in list
    int count = 0;
    StudentNode* temp = node->studentList;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    
    file << node->courseCode << "\t";
    file << node->courseName << "\t";
    file << node->lecturerName << "\t";
    file << node->credit << "\t";
    file << node->capacity << "\t";
    file << node->currentCapacity << "\t";
    file << count;
    
    // Write student list
    temp = node->studentList;
    while (temp != NULL) {
        file << "\n" << temp->matricNo << "\t" << temp->name;
        temp = temp->next;
    }
    file << "\n";
    
    saveCourseRec(node->right, file);
}

void saveStudents() {
    ofstream file("students.txt");
    if (!file) {
        cout << "Error saving students.\n";
        return;
    }
    
    Student* root = studentTree.getRoot();
    saveStudentRec(root, file);
    file.close();
}

void saveCourses() {
    ofstream file("courses.txt");
    if (!file) {
        cout << "Error saving courses.\n";
        return;
    }
    
    Course* root = courseTree.getRoot();
    saveCourseRec(root, file);
    file.close();
}

void savePendingRequests() {
    ofstream file("pending_requests.txt");
    if (!file) {
        cout << "Error saving pending requests.\n";
        return;
    }
    
    RegistrationRequest* temp = regQueue.getFront();
    while (temp != NULL) {
        if (!temp->isCancelled) {
            file << temp->matricNo << "\t";
            file << temp->studentName << "\t";
            file << temp->courseCode << "\t";
            file << temp->courseName << "\t";
            file << temp->credit << "\t";
            file << temp->timestamp << "\n";
        }
        temp = temp->next;
    }
    
    file.close();
}

void saveAllData() {
    cout << "\nSaving all data...\n";
    saveStudents();
    saveCourses();
    savePendingRequests();
    cout << "All data saved successfully.\n";
}

void markRequestAsCancelled(string matric, string courseCode) {
    RegistrationRequest* temp = regQueue.getFront();
    while (temp != NULL) {
        if (temp->matricNo == matric && temp->courseCode == courseCode) {
            temp->isCancelled = true;
            return;
        }
        temp = temp->next;
    }
}

void studentMenu(Student* student) {
    while (true) {
        cout << "\n========================================\n";
        cout << "   STUDENT MENU\n";
        cout << "========================================\n";
        cout << "Name: " << student->name << "\n";
        cout << "Matric No: " << student->matricNo << "\n";
        cout << "Program: " << student->program << " (Year " << student->year << ")\n";
        cout << "Credits: " << student->currentCredit << "/" << student->maxCredit << "\n";
        cout << "========================================\n";
        cout << "1. View All Courses\n";
        cout << "2. Search Course\n";
        cout << "3. Register Course\n";
        cout << "4. View Registered Courses\n";
        cout << "5. Delete Course\n";
        cout << "6. View Notifications\n";
        cout << "7. View Registration Slip\n";
        cout << "8. View Students in Course\n";
        cout << "9. Back to Main Menu\n";
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: viewAllCourses(); break;
            case 2: searchCourse(); break;
            case 3: registerCourse(student); break;
            case 4: viewRegisteredCourses(student); break;
            case 5: deleteCourse(student); break;
            case 6: viewNotifications(student); break;
            case 7: viewRegistrationSlip(student); break;
            case 8: viewStudentsInCourse(student); break;
            case 9: saveAllData(); return;
            default: cout << "Invalid choice.\n";
        }
    }
}

void staffMenu(Staff* staff) {
    while (true) {
        cout << "\n========================================\n";
        cout << "   STAFF MENU\n";
        cout << "========================================\n";
        cout << "1. View All Student Requests\n";
        cout << "2. View All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Insert Student\n";
        cout << "5. Update Course Details\n";
        cout << "6. Insert New Course\n";
        cout << "7. Delete Course\n";
        cout << "8. View Student List in Course\n";
        cout << "9. View All Courses\n";
        cout << "10. Retract Course from Student\n";
        cout << "11. Insert Course for Student\n";
        cout << "12. Back to Main Menu\n";
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: staffViewRequests(); break;
            case 2: staffViewAllStudents(); break;
            case 3: staffSearchStudent(); break;
            case 4: staffInsertStudent(); break;
            case 5: staffUpdateCourse(); break;
            case 6: staffInsertCourse(); break;
            case 7: staffDeleteCourse(); break;
            case 8: staffViewStudentListInCourse(); break;
            case 9: staffViewAllCourses(); break;
            case 10: staffRetractCourse(); break;
            case 11: staffInsertCourseForStudent(); break;
            case 12: saveAllData(); return;
            default: cout << "Invalid choice.\n";
        }
    }
}

void viewAllCourses() {
    courseTree.displayAll();
}

void searchCourse() {
    cout << "Enter Course Code: ";
    string code;
    getline(cin, code);
    
    Course* course = courseTree.search(code);
    if (course == NULL) {
        cout << "Course not found.\n";
    } else {
        cout << "\n=== Course Details ===\n";
        cout << "Code: " << course->courseCode << "\n";
        cout << "Name: " << course->courseName << "\n";
        cout << "Credit: " << course->credit << "\n";
        cout << "Students: " << course->currentCapacity << "/" << course->capacity << "\n";
    }
}

void registerCourse(Student* student) {
    cout << "Enter Course Code: ";
    string code;
    getline(cin, code);
    
    Course* course = courseTree.search(code);
    if (course == NULL) {
        cout << "Course not found.\n";
        return;
    }
    
    if (findCourseInStudent(student, code) != NULL) {
        cout << "You have already registered for this course.\n";
        return;
    }
    
    if (student->currentCredit + course->credit > student->maxCredit) {
        cout << "Registration failed: Exceeds maximum credit limit.\n";
        return;
    }
    
    if (course->currentCapacity >= course->capacity) {
        cout << "Course is already full.\n";
        return;
    }
    
    regQueue.enqueue(student->matricNo, student->name, course->courseCode, 
                     course->courseName, course->credit);
    addCourseToStudent(student, code, "Pending");
    student->currentCredit += course->credit;
    cout << "Registration request submitted successfully (Status: Pending).\n";
}

void viewRegisteredCourses(Student* student) {
    if (student->courseList == NULL) {
        cout << "No courses registered.\n";
        return;
    }
    
    cout << "\n=== Registered Courses ===\n";
    int count = 1;
    CourseNode* current = student->courseList;
    while (current != NULL) {
        Course* course = courseTree.search(current->courseCode);
        if (course != NULL) {
            cout << count++ << ". " << course->courseCode << " - " 
                 << course->courseName << " (" << course->credit 
                 << " credits) [" << current->status << "]\n";
        }
        current = current->next;
    }
}

void deleteCourse(Student* student) {
    viewRegisteredCourses(student);
    
    if (student->courseList == NULL) return;
    
    cout << "Enter Course Code to delete: ";
    string code;
    getline(cin, code);
    
    CourseNode* courseNode = findCourseInStudent(student, code);
    if (courseNode == NULL) {
        cout << "Course not found in your registered courses.\n";
        return;
    }
    
    Course* course = courseTree.search(code);
    if (course != NULL) {
        if (courseNode->status == "Pending") {
            student->currentCredit -= course->credit;
            markRequestAsCancelled(student->matricNo, code);
        } else {
            student->currentCredit -= course->credit;
            course->currentCapacity--;
            removeStudentFromCourseList(course, student->matricNo);
        }
    }
    
    removeCourseFromStudent(student, code);
    
    cout << "Course deleted successfully.\n";
}

void viewNotifications(Student* student) {
    while (true) {
        if (student->notifications.isEmpty()) {
            cout << "No notifications.\n";
            return;
        }
        
        Notification* notif = student->notifications.getTop();
        cout << "\n=== Latest Notification ===\n";
        cout << notif->message << "\n";
        cout << "Time: " << getCurrentTimestamp(notif->timestamp) << "\n";
        
        cout << "\n1. Mark as Unread (Stop)\n";
        cout << "2. Mark as Read (Next)\n";
        cout << "3. Mark All as Read\n";
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice == 1) {
            return;
        } else if (choice == 2) {
            student->notifications.pop();
        } else if (choice == 3) {
            student->notifications.clear();
            cout << "All notifications marked as read.\n";
            return;
        }
    }
}

void viewRegistrationSlip(Student* student) {
    cout << "\n========================================\n";
    cout << "      COURSE REGISTRATION SLIP\n";
    cout << "========================================\n";
    cout << "Name: " << student->name << "\n";
    cout << "Matric No: " << student->matricNo << "\n";
    cout << "Program: " << student->program << "\n";
    cout << "Year: " << student->year << "\n";
    cout << "========================================\n";
    cout << "APPROVED COURSES:\n";
    
    bool hasApproved = false;
    CourseNode* current = student->courseList;
    while (current != NULL) {
        if (current->status == "Approved") {
            Course* course = courseTree.search(current->courseCode);
            if (course != NULL) {
                cout << course->courseCode << " | " << course->courseName 
                     << " | " << course->credit << " credit(s)\n";
                hasApproved = true;
            }
        }
        current = current->next;
    }
    
    if (!hasApproved) {
        cout << "No approved courses yet.\n";
    }
    int approvedCredit = 0;
    CourseNode* cur = student->courseList;
    while (cur) {
        if (cur->status == "Approved") {
            Course* c = courseTree.search(cur->courseCode);
            if (c) approvedCredit += c->credit;
        }
        cur = cur->next;
    }
    cout << "========================================\n";
    cout << "Total Approved Credits: " << approvedCredit << "/" << student->maxCredit << "\n";
    
    cout << "\nDownload slip? (y/n): ";
    char choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 'y' || choice == 'Y') {
        string filename = student->matricNo + "_CourseRegistrationSlip.txt";
        ofstream file(filename.c_str());
        
        file << "========================================\n";
        file << "      COURSE REGISTRATION SLIP\n";
        file << "========================================\n";
        file << "Name: " << student->name << "\n";
        file << "Matric No: " << student->matricNo << "\n";
        file << "Program: " << student->program << "\n";
        file << "Year: " << student->year << "\n";
        file << "========================================\n";
        file << "APPROVED COURSES:\n";
        
        CourseNode* current = student->courseList;
        while (current != NULL) {
            if (current->status == "Approved") {
                Course* course = courseTree.search(current->courseCode);
                if (course != NULL) {
                    file << course->courseCode << " | " << course->courseName 
                         << " | " << course->credit << " credit(s)\n";
                }
            }
            current = current->next;
        }
        int approvedCredit = 0;
        CourseNode* cur = student->courseList;
        while (cur) {
            if (cur->status == "Approved") {
                Course* c = courseTree.search(cur->courseCode);
                if (c) approvedCredit += c->credit;
            }
            cur = cur->next;
        }
        file << "========================================\n";
        file << "Total Approved Credits: " << approvedCredit
            << "/" << student->maxCredit << "\n";
        file.close();
        cout << "Slip downloaded as " << filename << "\n";
    }
}

void viewStudentsInCourse(Student* student) {
    cout << "Enter Course Code: ";
    string code;
    getline(cin, code);
    
    Course* course = courseTree.search(code);
    if (course == NULL) {
        cout << "Course not found.\n";
        return;
    }
    
    bool isApproved = false;
    CourseNode* current = student->courseList;
    while (current != NULL) {
        if (current->courseCode == code && current->status == "Approved") {
            isApproved = true;
            break;
        }
        current = current->next;
    }
    
    if (!isApproved) {
        cout << "You are not approved in this course.\n";
        return;
    }
    
    cout << "\n=== Students in " << course->courseCode << " ===\n";
    if (course->studentList == NULL) {
        cout << "No students in this course.\n";
    } else {
        StudentNode* temp = course->studentList;
        int count = 1;
        while (temp != NULL) {
            cout << count++ << ". " << temp->name << " (" << temp->matricNo << ")\n";
            temp = temp->next;
        }
    }
}

void staffViewRequests() {
    if (regQueue.isEmpty()) {
        cout << "No pending requests.\n";
        return;
    }
    
    while (!regQueue.isEmpty()) {
        cout << "\n=== Pending Requests ===\n";
        regQueue.display();
        
        RegistrationRequest* req = regQueue.getFront();
        
        if (req->isCancelled) {
            cout << "\nRequest cancelled by student - skipping...\n";
            regQueue.dequeue();
            continue;
        }
        
        cout << "\nProcessing request for " << req->studentName 
             << " (" << req->matricNo << ")\n";
        cout << "Course: " << req->courseCode << " - " << req->courseName << "\n";
        cout << "1. Approve\n";
        cout << "2. Reject\n";
        cout << "3. Stop\n";
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice == 3) return;
        
        Student* student = studentTree.search(req->matricNo);
        Course* course = courseTree.search(req->courseCode);
        
        if (student != NULL && course != NULL) {
            CourseNode* courseNode = findCourseInStudent(student, req->courseCode);
            
            if (choice == 1) {
                if (courseNode != NULL) {
                    if (course->currentCapacity >= course->capacity) {
                        cout << "Approval failed: Course is full.\n";
                        student->currentCredit -= req->credit;
                        removeCourseFromStudent(student, req->courseCode);
                        string notifMsg = req->courseCode + " " + req->courseName + " auto-rejected (course full) at " + getCurrentTimestamp();
                        student->notifications.push(notifMsg);
                        regQueue.dequeue();
                        continue;
                    }
                    courseNode->status = "Approved";
                    course->currentCapacity++;
                    insertStudentInCourseList(course, student->matricNo, student->name);
                    string notifMsg = req->courseCode + " " + req->courseName + 
                                    " approved at " + getCurrentTimestamp();
                    student->notifications.push(notifMsg);
                    cout << "Request approved.\n";
                }
            } else if (choice == 2) {
                cout << "Enter rejection reason: ";
                string reason;
                getline(cin, reason);
                
                if (courseNode != NULL) {
                    student->currentCredit -= req->credit;
                    removeCourseFromStudent(student, req->courseCode);
                    string notifMsg = req->courseCode + " " + req->courseName + 
                                    " rejected at " + getCurrentTimestamp() + 
                                    ". Reason: " + reason;
                    student->notifications.push(notifMsg);
                    cout << "Request rejected.\n";
                }
            }
        }
        
        regQueue.dequeue();
        
        cout << "\nContinue processing? (y/n): ";
        char cont;
        cin >> cont;
        cin.ignore();
        
        if (cont != 'y' && cont != 'Y') return;
    }
    
    cout << "All requests processed.\n";
}

void staffViewAllStudents() {
    studentTree.displayAll();
}

void staffSearchStudent() {
    cout << "Enter Matric No: ";
    string matric;
    getline(cin, matric);
    
    Student* student = studentTree.search(matric);
    if (student == NULL) {
        cout << "Student not found.\n";
    } else {
        cout << "\n=== Student Details ===\n";
        cout << "Name: " << student->name << "\n";
        cout << "Matric No: " << student->matricNo << "\n";
        cout << "Program: " << student->program << "\n";
        cout << "Year: " << student->year << "\n";
        cout << "Credits: " << student->currentCredit << "/" << student->maxCredit << "\n";
        
        cout << "\n=== Registered Courses ===\n";
        if (student->courseList == NULL) {
            cout << "No courses registered.\n";
        } else {
            CourseNode* current = student->courseList;
            while (current != NULL) {
                Course* course = courseTree.search(current->courseCode);
                if (course != NULL) {
                    cout << course->courseCode << " - " << course->courseName 
                         << " [" << current->status << "]\n";
                }
                current = current->next;
            }
        }
    }
}

void staffInsertStudent() {
    string studentName, matricNo, program;
    int maxCredit, currentCredit, year;
    cout << "Enter Student Name: ";
    getline(cin, studentName);
    cout << "Enter Matric No: ";
    getline(cin, matricNo);
    cout << "Enter Max Credit: ";
    cin >> maxCredit;
    cout << "Enter Year: ";
    cin >> year;
    cin.ignore();
    cout << "Enter Program: ";
    getline(cin, program);
    currentCredit = 0;
    Student* newStudent = new Student(studentName, matricNo, maxCredit, currentCredit, year, program);
    
    // cannot insert duplicate matric no
    if (studentTree.search(newStudent->matricNo) != NULL) {
        cout << "Student with this Matric No already exists.\n";
        delete newStudent;
        return;
    }
    studentTree.insert(newStudent);
    cout << "Student inserted successfully.\n";
}

void staffUpdateCourse() {
    cout << "Enter Course Code: ";
    string code;
    getline(cin, code);
    
    Course* course = courseTree.search(code);
    if (course == NULL) {
        cout << "Course not found.\n";
        return;
    }
    
    cout << "\nCurrent Details:\n";
    cout << "Lecturer: " << course->lecturerName << "\n";
    cout << "Capacity: " << course->capacity << "\n";
    
    cout << "\n1. Update Lecturer\n";
    cout << "2. Update Capacity\n";
    cout << "3. Update Both\n";
    cout << "Choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 1 || choice == 3) {
        cout << "Enter New Lecturer Name: ";
        getline(cin, course->lecturerName);
    }
    
    if (choice == 2 || choice == 3) {
        cout << "Enter New Capacity: ";
        cin >> course->capacity;
        cin.ignore();
    }
    
    cout << "Course updated successfully.\n";
}

void staffInsertCourse() {
    string courseCode, courseName, lecturerName;
    int credit, capacity, currentCapacity;
    cout << "Enter Course Code: ";
    getline(cin, courseCode);
    cout << "Enter Course Name: ";
    getline(cin, courseName);
    cout << "Enter Lecturer Name: ";
    getline(cin, lecturerName);
    cout << "Enter Credit: ";
    cin >> credit;
    cout << "Enter Capacity: ";
    cin >> capacity;
    cin.ignore();
    currentCapacity = 0;
    Course* newCourse = new Course(courseCode, courseName, lecturerName, credit, capacity, currentCapacity);
    // cannot insert duplicate course code
    if (courseTree.search(newCourse->courseCode) != NULL) {
        cout << "Course with this code already exists.\n";
        delete newCourse;
        return;
    }
    courseTree.insert(newCourse);
    cout << "Course inserted successfully.\n";
}

void staffDeleteCourse() {
    cout << "Enter Course Code: ";
    string code;
    getline(cin, code);
    
    Course* course = courseTree.search(code);
    if (course == NULL) {
        cout << "Course not found.\n";
        return;
    }
    // if course have student cannot delete
    if (course->studentList != NULL) {
        cout << "Cannot delete course with enrolled students.\n";
        return;
    }
    courseTree.deleteCourse(code);
    cout << "Course deleted successfully.\n";
}

void staffViewStudentListInCourse() {
    cout << "Enter Course Code: ";
    string code;
    getline(cin, code);
    
    Course* course = courseTree.search(code);
    if (course == NULL) {
        cout << "Course not found.\n";
        return;
    }
    
    cout << "\n=== Students in " << course->courseCode << " - " << course->courseName << " ===\n";
    
    if (course->studentList == NULL) {
        cout << "No students in this course.\n";
    } else {
        StudentNode* temp = course->studentList;
        int count = 1;
        while (temp != NULL) {
            cout << count++ << ". " << temp->name << " (" << temp->matricNo << ")\n";
            temp = temp->next;
        }
    }
    
    cout << "\nDownload list? (y/n): ";
    char choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 'y' || choice == 'Y') {
        string filename = course->courseCode + "_StudentList.txt";
        ofstream file(filename.c_str());
        
        file << "=== Students in " << course->courseCode << " - " << course->courseName << " ===\n\n";
        
        StudentNode* temp = course->studentList;
        int count = 1;
        while (temp != NULL) {
            file << count++ << ". " << temp->name << " (" << temp->matricNo << ")\n";
            temp = temp->next;
        }
        
        file.close();
        cout << "List downloaded as " << filename << "\n";
    }
}

void staffViewAllCourses() {
    courseTree.displayAll();
}

void staffRetractCourse() {
    cout << "Enter Student Matric No: ";
    string matric;
    getline(cin, matric);
    
    Student* student = studentTree.search(matric);
    if (student == NULL) {
        cout << "Student not found.\n";
        return;
    }
    
    cout << "\n=== Student: " << student->name << " (" << student->matricNo << ") ===\n";
    cout << "Registered Courses:\n";
    
    if (student->courseList == NULL) {
        cout << "No courses registered.\n";
        return;
    }
    
    int count = 1;
    CourseNode* current = student->courseList;
    while (current != NULL) {
        Course* course = courseTree.search(current->courseCode);
        if (course != NULL) {
            cout << count++ << ". " << course->courseCode << " - " 
                 << course->courseName << " [" << current->status << "]\n";
        }
        current = current->next;
    }
    
    cout << "\nEnter Course Code to retract: ";
    string code;
    getline(cin, code);
    
    CourseNode* courseNode = findCourseInStudent(student, code);
    if (courseNode == NULL) {
        cout << "Course not found in student's registered courses.\n";
        return;
    }
    
    Course* course = courseTree.search(code);
    if (course != NULL) {
        student->currentCredit -= course->credit;
        
        if (courseNode->status == "Approved") {
            removeStudentFromCourseList(course, matric);
            course->currentCapacity--;
        }
    }
    
    removeCourseFromStudent(student, code);
    
    cout << "Course retracted successfully.\n";
    cout << "\nUpdated Registered Courses:\n";
    
    if (student->courseList == NULL) {
        cout << "No courses registered.\n";
    } else {
        CourseNode* current = student->courseList;
        while (current != NULL) {
            Course* c = courseTree.search(current->courseCode);
            if (c != NULL) {
                cout << c->courseCode << " - " << c->courseName 
                     << " [" << current->status << "]\n";
            }
            current = current->next;
        }
    }
}

void staffInsertCourseForStudent() {
    cout << "Enter Student Matric No: ";
    string matric;
    getline(cin, matric);
    
    Student* student = studentTree.search(matric);
    if (student == NULL) {
        cout << "Student not found.\n";
        return;
    }
    
    cout << "\n=== Student: " << student->name << " (" << student->matricNo << ") ===\n";
    cout << "Current Registered Courses:\n";
    
    if (student->courseList == NULL) {
        cout << "No courses registered.\n";
    } else {
        CourseNode* current = student->courseList;
        while (current != NULL) {
            Course* c = courseTree.search(current->courseCode);
            if (c != NULL) {
                cout << c->courseCode << " - " << c->courseName 
                     << " [" << current->status << "]\n";
            }
            current = current->next;
        }
    }
    
    cout << "\nEnter Course Code to insert: ";
    string code;
    getline(cin, code);
    
    Course* course = courseTree.search(code);
    if (course == NULL) {
        cout << "Course not found.\n";
        return;
    }
    
    if (findCourseInStudent(student, code) != NULL) {
        cout << "Student already registered for this course.\n";
        return;
    }
    
    if (student->currentCredit + course->credit > student->maxCredit) {
        cout << "Cannot insert: Exceeds maximum credit limit.\n";
        return;
    }
    
    if (course->currentCapacity >= course->capacity) {
        cout << "Cannot insert: Course is full.\n";
        return;
    }
    
    addCourseToStudent(student, code, "Approved");
    student->currentCredit += course->credit;
    insertStudentInCourseList(course, student->matricNo, student->name);
    course->currentCapacity++;
    
    cout << "Course inserted successfully for student.\n";
    cout << "\nUpdated Registered Courses:\n";
    
    CourseNode* current = student->courseList;
    while (current != NULL) {
        Course* c = courseTree.search(current->courseCode);
        if (c != NULL) {
            cout << c->courseCode << " - " << c->courseName 
                 << " [" << current->status << "]\n";
        }
        current = current->next;
    }
}

void insertStudentInCourseList(Course* course, string matric, string name) {
    StudentNode* newNode = new StudentNode(matric, name);
    
    if (course->studentList == NULL || course->studentList->matricNo > matric) {
        newNode->next = course->studentList;
        course->studentList = newNode;
    } else {
        StudentNode* temp = course->studentList;
        while (temp->next != NULL && temp->next->matricNo < matric) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

void removeStudentFromCourseList(Course* course, string matric) {
    if (course->studentList == NULL) return;
    
    if (course->studentList->matricNo == matric) {
        StudentNode* temp = course->studentList;
        course->studentList = course->studentList->next;
        delete temp;
        return;
    }
    
    StudentNode* temp = course->studentList;
    while (temp->next != NULL && temp->next->matricNo != matric) {
        temp = temp->next;
    }
    
    if (temp->next != NULL) {
        StudentNode* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
    }
}

string getCurrentTimestamp(time_t t) {
    time_t now = (t == 0) ? std::time(nullptr) : t;
    tm* local = std::localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);
    return buffer;
}

void easterEgg() {
    int num=0;
    if (rand() % 1000 == 0) {
        cout << "500 Internal Server Error\n";
        exit(0);
    }
}
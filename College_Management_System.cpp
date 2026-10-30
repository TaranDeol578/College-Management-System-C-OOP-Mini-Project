// COLLEGE MANAGEMENT SYSTEM - C++ OOP MINI PROJECT
// Prepared by: TARANJOT SINGH | UID: 25BCD10103
// Course code: 25CAH-201
// Compile: g++ -std=c++17 College_Management_System.cpp -o college_manager
// Run: ./college_manager  (Windows: college_manager.exe)

#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
using namespace std;

// Base class: abstraction and runtime polymorphism.
class Person {
protected:
    int id;
    string name;
public:
    Person(int number, string fullName) : id(number), name(fullName) {}
    virtual ~Person() = default;
    int getId() const { return id; }
    string getName() const { return name; }
    virtual void display() const = 0;
};

// Derived class: inheritance from Person.
class Student : public Person {
    string program;
    int semester;
public:
    Student(int number, string fullName, string course, int sem)
        : Person(number, fullName), program(course), semester(sem) {}
    void display() const override {
        cout << "ID: " << id << " | Name: " << name
             << " | Program: " << program
             << " | Semester: " << semester << '\n';
    }
    void save(ostream& out) const {
        out << id << ' ' << quoted(name) << ' ' << quoted(program)
            << ' ' << semester << '\n';
    }
};

// Derived class: inheritance and method overriding.
class Faculty : public Person {
    string department;
    string designation;
public:
    Faculty(int number, string fullName, string dept, string job)
        : Person(number, fullName), department(dept), designation(job) {}
    void display() const override {
        cout << "ID: " << id << " | Name: " << name
             << " | Department: " << department
             << " | Designation: " << designation << '\n';
    }
    void save(ostream& out) const {
        out << id << ' ' << quoted(name) << ' '
            << quoted(department) << ' ' << quoted(designation) << '\n';
    }
};

class Course {
    string code;
    string title;
    int credits;
    int facultyId;
public:
    Course(string courseCode, string courseTitle, int creditCount, int teacherId)
        : code(courseCode), title(courseTitle), credits(creditCount), facultyId(teacherId) {}
    string getCode() const { return code; }
    void display() const {
        cout << "Code: " << code << " | Title: " << title
             << " | Credits: " << credits
             << " | Faculty ID: " << facultyId << '\n';
    }
    void save(ostream& out) const {
        out << quoted(code) << ' ' << quoted(title)
            << ' ' << credits << ' ' << facultyId << '\n';
    }
};

struct Enrollment {
    int studentId;
    string courseCode;
    string grade; // NA until a grade is recorded.
};

class CollegeManagement {
    vector<Student> students;
    vector<Faculty> faculty;
    vector<Course> courses;
    vector<Enrollment> enrollments;

    static int readInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) return value;
            if (cin.eof()) return -1;
            cout << "Invalid number. Please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    static string readText(const string& prompt) {
        cout << prompt;
        string answer;
        getline(cin >> ws, answer);
        return answer;
    }
    Student* findStudent(int number) {
        for (auto& item : students)
            if (item.getId() == number) return &item;
        return nullptr;
    }
    Faculty* findFaculty(int number) {
        for (auto& item : faculty)
            if (item.getId() == number) return &item;
        return nullptr;
    }
    Course* findCourse(const string& code) {
        for (auto& item : courses)
            if (item.getCode() == code) return &item;
        return nullptr;
    }
    void load() {
        int id, sem, credits, teacherId;
        string name, program, dept, designation, code, title, grade;
        ifstream s("students.txt");
        while (s >> id >> quoted(name) >> quoted(program) >> sem)
            students.emplace_back(id, name, program, sem);
        ifstream f("faculty.txt");
        while (f >> id >> quoted(name) >> quoted(dept) >> quoted(designation))
            faculty.emplace_back(id, name, dept, designation);
        ifstream c("courses.txt");
        while (c >> quoted(code) >> quoted(title) >> credits >> teacherId)
            courses.emplace_back(code, title, credits, teacherId);
        ifstream e("enrollments.txt");
        while (e >> id >> quoted(code) >> quoted(grade))
            enrollments.push_back({id, code, grade});
    }
    void save() const {
        ofstream s("students.txt"), f("faculty.txt");
        ofstream c("courses.txt"), e("enrollments.txt");
        if (!s || !f || !c || !e) {
            cerr << "Error: Could not write data files.\n";
            return;
        }
        for (const auto& item : students) item.save(s);
        for (const auto& item : faculty) item.save(f);
        for (const auto& item : courses) item.save(c);
        for (const auto& item : enrollments)
            e << item.studentId << ' ' << quoted(item.courseCode)
              << ' ' << quoted(item.grade) << '\n';
    }
    void addStudent() {
        int id = readInt("Enter student ID: ");
        if (id <= 0 || findStudent(id)) {
            cout << "Invalid or duplicate student ID.\n"; return;
        }
        string name = readText("Enter name: ");
        string program = readText("Enter program: ");
        int semester = readInt("Enter semester (1-8): ");
        if (semester < 1 || semester > 8) {
            cout << "Semester must be between 1 and 8.\n"; return;
        }
        students.emplace_back(id, name, program, semester);
        save();
        cout << "Student added successfully.\n";
    }
    void addFaculty() {
        int id = readInt("Enter faculty ID: ");
        if (id <= 0 || findFaculty(id)) {
            cout << "Invalid or duplicate faculty ID.\n"; return;
        }
        string name = readText("Enter faculty name: ");
        string dept = readText("Enter department: ");
        string job = readText("Enter designation: ");
        faculty.emplace_back(id, name, dept, job);
        save();
        cout << "Faculty added successfully.\n";
    }
    void addCourse() {
        string code = readText("Enter course code: ");
        if (findCourse(code)) {
            cout << "Course code already exists.\n"; return;
        }
        string title = readText("Enter course title: ");
        int credits = readInt("Enter credits (1-6): ");
        int facultyId = readInt("Enter assigned faculty ID: ");
        if (credits < 1 || credits > 6 || !findFaculty(facultyId)) {
            cout << "Invalid credits or faculty ID.\n"; return;
        }
        courses.emplace_back(code, title, credits, facultyId);
        save();
        cout << "Course added successfully.\n";
    }
    void enrollStudent() {
        int id = readInt("Enter student ID: ");
        string code = readText("Enter course code: ");
        if (!findStudent(id) || !findCourse(code)) {
            cout << "Student or course not found.\n"; return;
        }
        for (const auto& item : enrollments)
            if (item.studentId == id && item.courseCode == code) {
                cout << "Student is already enrolled.\n"; return;
            }
        enrollments.push_back({id, code, "NA"});
        save();
        cout << "Enrollment successful.\n";
    }
    void recordGrade() {
        int id = readInt("Enter student ID: ");
        string code = readText("Enter course code: ");
        for (auto& item : enrollments) {
            if (item.studentId == id && item.courseCode == code) {
                string grade = readText("Enter grade (A/B/C/D/F): ");
                if (grade != "A" && grade != "B" && grade != "C"
                    && grade != "D" && grade != "F") {
                    cout << "Invalid grade.\n"; return;
                }
                item.grade = grade;
                save();
                cout << "Grade saved successfully.\n"; return;
            }
        }
        cout << "Enrollment not found.\n";
    }
    void viewStudents() const {
        cout << "\n--- STUDENT RECORDS ---\n";
        if (students.empty()) cout << "No records found.\n";
        // Calls overridden display(): runtime polymorphism.
        for (const auto& student : students) {
            const Person& person = student;
            person.display();
        }
    }
    void viewFaculty() const {
        cout << "\n--- FACULTY RECORDS ---\n";
        if (faculty.empty()) cout << "No records found.\n";
        for (const auto& teacher : faculty) {
            const Person& person = teacher;
            person.display();
        }
    }
    void viewCourses() const {
        cout << "\n--- COURSE RECORDS ---\n";
        if (courses.empty()) cout << "No records found.\n";
        for (const auto& course : courses) course.display();
    }
    void viewEnrollments() const {
        cout << "\n--- ENROLLMENT RECORDS ---\n";
        if (enrollments.empty()) cout << "No records found.\n";
        for (const auto& entry : enrollments)
            cout << "Student ID: " << entry.studentId
                 << " | Course: " << entry.courseCode
                 << " | Grade: " << entry.grade << '\n';
    }
    void searchStudent() {
        int id = readInt("Enter student ID to search: ");
        Student* student = findStudent(id);
        if (student) student->display();
        else cout << "Student not found.\n";
    }
    void summary() const {
        cout << "\n--- COLLEGE SUMMARY ---\n"
             << "Total students: " << students.size() << '\n'
             << "Total faculty: " << faculty.size() << '\n'
             << "Total courses: " << courses.size() << '\n'
             << "Total enrollments: " << enrollments.size() << '\n';
    }
public:
    CollegeManagement() { load(); }
    void run() {
        int choice;
        do {
            cout << "\n========== COLLEGE MANAGEMENT SYSTEM ==========\n"
                 << "1. Add Student            2. Add Faculty\n"
                 << "3. Add Course             4. Enroll Student\n"
                 << "5. Record Grade           6. View Students\n"
                 << "7. View Faculty           8. View Courses\n"
                 << "9. View Enrollments      10. Search Student\n"
                 << "11. Summary Report       12. Exit\n";
            choice = readInt("Enter your choice: ");
            switch (choice) {
                case 1: addStudent(); break;
                case 2: addFaculty(); break;
                case 3: addCourse(); break;
                case 4: enrollStudent(); break;
                case 5: recordGrade(); break;
                case 6: viewStudents(); break;
                case 7: viewFaculty(); break;
                case 8: viewCourses(); break;
                case 9: viewEnrollments(); break;
                case 10: searchStudent(); break;
                case 11: summary(); break;
                case 12: cout << "Data saved. Goodbye!\n"; break;
                default: cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 12 && choice != -1);
    }
};

int main() {
    CollegeManagement college;
    college.run();
    return 0;
}

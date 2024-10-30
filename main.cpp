#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <numeric>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;

bool is_valid_student_id(const string& id) {
    return id.length() == 8 && all_of(id.begin(), id.end(), ::isdigit);
}

class Student {
public:
    string last_name;
    string first_name;
    string student_id;

    Student(string last, string first, string id) : last_name(last), first_name(first), student_id(id) {}
};

class Assignment {
public:
    string name;
    int total_points;
    string category;

    Assignment(string n, int points, string cat) : name(n), total_points(points), category(cat) {}
};

class Gradebook {
public:
    vector<Student> students;
    vector<Assignment> assignments;
    map<string, map<string, int>> grades;

    void add_student(string last, string first, string id) {
        if (!is_valid_student_id(id)) {
            cout << "\033[31mInvalid student ID. ID must be 8 digits.\033[0m\n";
            return;
        }
        // Check for duplicate student ID
        for (const auto& student : students) {
            if (student.student_id == id) {
                cout << "\033[31mStudent ID already exists. Please use a unique ID.\033[0m\n";
                return;
            }
        }
        students.emplace_back(last, first, id);
    }

    void add_assignment(string name, int points, const vector<string>& student_ids, string category) {
        if (name.empty()) {
            name = "Test Quiz"; // Default assignment name
            cout << "\033[33mNo assignment name provided. Defaulting to 'Test Quiz'.\033[0m\n";
        }
        if (category.empty()) {
            category = "Uncategorized"; // Default category
            cout << "\033[33mNo category provided. Defaulting to 'Uncategorized'.\033[0m\n";
        } else {
            // Convert category to lowercase
            transform(category.begin(), category.end(), category.begin(), ::tolower);
        }
        assignments.emplace_back(name, points, category);
        for (const auto& student_id : student_ids) {
            grades[student_id][name] = 0;
        }
    }

    void enter_grade(string id, string assignment_name, int grade) {
        grades[id][assignment_name] = grade;
    }

    string report() {
        string report_str = "\033[34mLast_Name,First_Name,Student_Id\033[0m";
        for (const auto& assignment : assignments) {
            report_str += "\033[34m," + assignment.name + "\033[0m";
        }
        report_str += "\033[34m,Average\033[0m\n";

        for (const auto& student : students) {
            report_str += "\033[32m" + student.last_name + "," + student.first_name + "," + student.student_id + "\033[0m";
            double total_earned = 0;
            double total_possible = 0;

            for (const auto& assignment : assignments) {
                if (grades.count(student.student_id) && grades[student.student_id].count(assignment.name)) {
                    int earned_points = grades[student.student_id][assignment.name];
                    report_str += "\033[32m," + to_string(earned_points) + "\033[0m";
                    total_earned += earned_points;
                    total_possible += assignment.total_points;
                } else {
                    report_str += "\033[32m,0\033[0m";
                    total_possible += assignment.total_points;
                }
            }

            if (total_possible > 0) {
                double average = total_earned / total_possible * 100.0;
                report_str += "\033[32m," + to_string(average).substr(0, to_string(average).find(".") + 3) + "\033[0m";
            } else {
                report_str += "\033[32m,none\033[0m";
            }
            report_str += "\n";
        }

        return report_str;
    }

    string assignment_report(const string& assignment_name) {
      string report = "\033[34mLast_Name,First_Name,Student_Id,Score\033[0m\n";
      double total_score = 0;
      int num_graded = 0;

      for (const auto& student : students) {
          report += "\033[32m" + student.last_name + "," + student.first_name + "," + student.student_id + "\033[0m";
          if (grades[student.student_id].count(assignment_name)) {
              int score = grades[student.student_id][assignment_name];
              report += "\033[32m," + to_string(score) + "\033[0m\n";
              total_score += score;
              num_graded++;
          } else {
              report += "\033[32m,none\033[0m\n";
          }
      }

        report += "\n";
        if (num_graded > 0) {
            double average = total_score / num_graded;
            report += "\033[34mAverage score: " + to_string(average) + "\033[0m";
            for (const auto& assignment : assignments) {
                if (assignment.name == assignment_name) {
                    report += "\033[34m / " + to_string(assignment.total_points) + "\033[0m";
                    break;
                }
            }
        } else {
            report += "\033[34mAverage score: none\033[0m";
        }
        return report;
    }

    bool is_valid_student_id(const string& id) {
        return id.length() == 8 && all_of(id.begin(), id.end(), ::isdigit);
    }
};

void display_welcome_message() {
    cout << "\033[36mWelcome to the Gradebook Management System!\033[0m\n";
    cout << "\033[36mThis program allows you to manage student records and assignments.\033[0m\n";
    cout << "\033[36mYou can perform the following actions:\033[0m\n";
    cout << "\033[32m1. Add a new student with a unique 8-digit ID.\033[0m\n";
    cout << "\033[32m2. Add assignments with categories like Finals, Tests, Quizzes, and Midterms.\033[0m\n";
    cout << "\033[32m3. Enter grades for students for specific assignments.\033[0m\n";
    cout << "\033[32m4. Generate a report of all students' grades and averages.\033[0m\n";
    cout << "\033[32m5. Generate a report for a specific assignment.\033[0m\n";
    cout << "\033[31m6. Exit the program.\033[0m\n";
    cout << "\033[36mPlease follow the prompts carefully to ensure correct data entry.\033[0m\n";
    cout << "\033[36mPress Enter to continue...\033[0m\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Wait for user to press Enter
}

int main() {
    Gradebook gradebook;
    int choice;

    display_welcome_message(); // Display the welcome message

    while (true) {
        cout << "\nGradebook Menu:\n";
        cout << "\033[32m1. Add Student\033[0m\n";
        cout << "\033[36m2. Add Assignment\033[0m\n";
        cout << "\033[31m3. Enter Grade\033[0m\n";
        cout << "\033[33m4. Generate Report\033[0m\n";
        cout << "\033[34m5. Generate Assignment Report\033[0m\n";
        cout << "\033[35m6. Exit\033[0m\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string last_name, first_name, student_id;
                do {
                    cout << "Enter last name: ";
                    cin >> ws;
                    getline(cin, last_name);
                    if (last_name.empty()) {
                        cout << "\033[31mLast name cannot be empty. Please try again.\033[0m\n";
                    }
                } while (last_name.empty());

                do {
                    cout << "Enter first name: ";
                    getline(cin, first_name);
                    if (first_name.empty()) {
                        cout << "\033[31mFirst name cannot be empty. Please try again.\033[0m\n";
                    }
                } while (first_name.empty());

                do {
                    cout << "Enter student ID (8 digits): ";
                    getline(cin, student_id);
                    if (student_id.empty()) {
                        cout << "\033[31mStudent ID cannot be empty. Please try again.\033[0m\n";
                    }
                } while (student_id.empty());

                gradebook.add_student(last_name, first_name, student_id);
                break;
            }
            case 2: {
                string assignment_name;
                int total_points;
                vector<string> assigned_students;
                string category;

                do {
                    cout << "Enter assignment name: ";
                    cin.ignore();
                    getline(cin, assignment_name);
                    if (assignment_name.empty()) {
                        cout << "\033[31mAssignment name cannot be empty. Please try again.\033[0m\n";
                    }
                } while (assignment_name.empty());

                cout << "Enter total points: ";
                while (!(cin >> total_points)) {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                do {
                    cout << "Enter assignment category (e.g., Finals, Tests, Quizzes, Midterms): ";
                    cin.ignore();
                    getline(cin, category);
                    if (category.empty()) {
                        cout << "\033[31mCategory cannot be empty. Please try again.\033[0m\n";
                    }
                } while (category.empty());

                // Convert category to lowercase
                transform(category.begin(), category.end(), category.begin(), ::tolower);

                cout << "\nAvailable Students:\n";
                for (size_t i = 0; i < gradebook.students.size(); ++i) {
                    cout << i + 1 << ". " << gradebook.students[i].first_name << " "
                              << gradebook.students[i].last_name << " (" << gradebook.students[i].student_id << ")\n";
                }

                int student_choice;
                do {
                    cout << "Select student to assign to (enter student ID, or 0 to finish): ";
                    string student_id_input;
                    cin >> student_id_input;

                    if (student_id_input == "0") {
                        break;
                    }

                    if (gradebook.is_valid_student_id(student_id_input)) {
                        bool student_found = false;
                        for (const auto& student : gradebook.students) {
                            if (student.student_id == student_id_input) {
                                assigned_students.push_back(student_id_input);
                                student_found = true;
                                cout << "\033[32mStudent ID " << student_id_input << " confirmed and assigned.\033[0m\n";
                                break;
                            }
                        }
                        if (!student_found) {
                            cout << "\033[31mStudent ID not found.\033[0m\n";
                        }
                    } else {
                        cout << "\033[31mInvalid student ID format. Please enter 8 digits.\033[0m\n";
                    }

                } while (true);

                gradebook.add_assignment(assignment_name, total_points, assigned_students, category);
                break;
            }
            case 3: {
                string student_id, assignment_name;
                int grade;
                do {
                    cout << "Enter student ID: ";
                    cin >> ws;
                    getline(cin, student_id);
                    if (student_id.empty()) {
                        cout << "\033[31mStudent ID cannot be empty. Please try again.\033[0m\n";
                    }
                } while (student_id.empty());

                do {
                    cout << "Enter assignment name: ";
                    getline(cin, assignment_name);
                    if (assignment_name.empty()) {
                        cout << "\033[31mAssignment name cannot be empty. Please try again.\033[0m\n";
                    }
                } while (assignment_name.empty());

                cout << "Enter grade: ";
                while (!(cin >> grade)) {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                gradebook.enter_grade(student_id, assignment_name, grade);
                break;
            }
            case 4: {
                cout << gradebook.report() << endl;
                break;
            }
            case 5: {
                string assignment_name;
                do {
                    cout << "Enter assignment name: ";
                    cin.ignore();
                    getline(cin, assignment_name);
                    if (assignment_name.empty()) {
                        cout << "\033[31mAssignment name cannot be empty. Please try again.\033[0m\n";
                    }
                } while (assignment_name.empty());
                cout << gradebook.assignment_report(assignment_name) << endl;
                break;
            }
            case 6: {
                cout << "\033[37mExiting...\033[0m\n";
                return 0;
            }
            default: {
                cout << "\033[31mInvalid choice. Please try again.\033[0m\n";
                break;
            }
        }
    }

    return 0;
}

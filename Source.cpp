#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <algorithm>
#include <string>

class Course {
public:
    std::string courseNumber;
    std::string courseTitle;
    std::vector<std::string> prerequisites;

    Course(std::string number, std::string title, std::vector<std::string> prereqs)
        : courseNumber(number), courseTitle(title), prerequisites(prereqs) {}
};

// Function to count the total number of prerequisite courses for a given course
int numPrerequisiteCourses(const std::unordered_map<std::string, Course>& courses, const std::string& courseNumber) {
    int totalPrerequisites = 0;
    //checks for course using courseNumber
    if (courses.find(courseNumber) == courses.end()) {
        std::cout << "Course not found." << std::endl;
        return 0;
    }
    //depth first traverse
    std::stack<Course> courseStack;
    courseStack.push(courses.at(courseNumber)); //push the initial course onto the stack

    while (!courseStack.empty()) {
        //pop the top course
        Course currentCourse = courseStack.top();
        courseStack.pop();

        totalPrerequisites += currentCourse.prerequisites.size();
        //pushes a prerequisite onto the stack for later
        for (const auto& prerequisite : currentCourse.prerequisites) {
            if (courses.find(prerequisite) != courses.end()) {
                courseStack.push(courses.at(prerequisite));
            }
        }
    }

    std::cout << "Total number of prerequisite courses for " << courseNumber << ": " << totalPrerequisites << std::endl;
    return totalPrerequisites;
}

// Function to print a sample schedule of courses
void printSampleSchedule(const std::unordered_map<std::string, Course>& courses) {
    for (const auto& coursePair : courses) {
        std::cout << coursePair.second.courseNumber << ": " << coursePair.second.courseTitle << std::endl;
    }
}

// Function to print course information and prerequisites
void printCourseInformation(const std::unordered_map<std::string, Course>& courses, const std::string& courseNumber) {
    //checks for course using courseNumber
    if (courses.find(courseNumber) == courses.end()) {
        std::cout << "Course not found." << std::endl;
        return;
    }
    //display course information(course number and title)
    const Course& course = courses.at(courseNumber);
    std::cout << "Course Number: " << course.courseNumber << std::endl;
    std::cout << "Title: " << course.courseTitle << std::endl;
    //check for prerequisites
    if (course.prerequisites.empty()) {
        std::cout << "Prerequisites: None" << std::endl;
    }
    else {
        std::cout << "Prerequisites:" << std::endl;
        for (const auto& prerequisite : course.prerequisites) {
            std::cout << "   " << prerequisite << std::endl;
        }
    }
}

// Function to load course data from a file into the data structure
void loadDataStructure(const std::string& fileName, std::unordered_map<std::string, Course>& dataStructure) {
    //open file
    std::ifstream file(fileName); 
    std::string line;

    //loop to read each line
    while (std::getline(file, line)) {
        // Parse line to extract course information
        std::vector<std::string> tokens;
        size_t start = 0, end;
        while ((end = line.find(',', start)) != std::string::npos) {
            tokens.push_back(line.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(line.substr(start));


        if (tokens.size() < 2) {
            std::cerr << "Invalid data format in the file." << std::endl;
            continue;
        }

        std::string courseNumber = tokens[0];
        std::string courseTitle = tokens[1];
        std::vector<std::string> prerequisites(tokens.begin() + 2, tokens.end());
        //add "course" object t hash table
        dataStructure.emplace(courseNumber, Course(courseNumber, courseTitle, prerequisites));
    }

    file.close();
}

// Function to print the list of courses
void printCourseList(const std::unordered_map<std::string, Course>& dataStructure) {
    //empty vector
    std::vector<std::string> sortedCourses;
    sortedCourses.reserve(dataStructure.size());
    //loop to go through dataStructure and obtain course number
    for (const auto& coursePair : dataStructure) {
        sortedCourses.push_back(coursePair.first);
    }
    //sorts list
    std::sort(sortedCourses.begin(), sortedCourses.end());
    //prints course information
    for (const std::string& courseNumber : sortedCourses) {
        const Course& course = dataStructure.at(courseNumber);
        std::cout << "Course Number: " << course.courseNumber << ", Title: " << course.courseTitle << std::endl;
    }
}

// Function to find a course by course code
const Course* findCourse(const std::string& courseCode, const std::unordered_map<std::string, Course>& dataStructure) {
    auto it = dataStructure.find(courseCode);
    if (it != dataStructure.end()) {
        return &(it->second);
    }
    return nullptr;
}

// Function to print course information and prerequisites
void printCourse(const std::string& courseCode, const std::unordered_map<std::string, Course>& dataStructure) {
    const Course* course = findCourse(courseCode, dataStructure);
    //course found
    if (course != nullptr) {
        std::cout << "Course Title: " << course->courseTitle << std::endl;
        //course prerequisites are not empty
        if (!course->prerequisites.empty()) {
            std::cout << "Prerequisites:" << std::endl;
            //loop that prints prerequisites
            for (const auto& prerequisite : course->prerequisites) {
                std::cout << "   " << prerequisite << std::endl;
            }
        }
    }
    //course not found
    else {
        std::cout << "Course not found." << std::endl;
    }
}


int main() {
    std::unordered_map<std::string, Course> dataStructure;  //hash table innit
    int choice;

    while (true) {
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Load Data Structure" << std::endl;
        std::cout << "2. Print Course List" << std::endl;
        std::cout << "3. Print Course" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string fileName;
            std::cout << "Enter file name: ";
            std::cin >> fileName;
            loadDataStructure(fileName, dataStructure);
        }
        else if (choice == 2) {
            printCourseList(dataStructure);
        }
        else if (choice == 3) {
            std::string courseCode;
            std::cout << "Enter course code: ";
            std::cin >> courseCode;
            printCourse(courseCode, dataStructure);
        }
        else if (choice == 4) {
            std::cout << "Exiting program." << std::endl;
            break;
        }
        else {
            std::cout << "Invalid choice. Please choose a valid option." << std::endl;
        }
    }

    return 0; //return 0 to indicate successful program execution
}

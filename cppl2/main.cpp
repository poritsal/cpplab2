#include "professor.h"

int main() {
    srand(time(nullptr));    

    std::fstream studentsfile = SafeOpen("students.txt");
    std::vector<std::string> studentsList = Students(studentsfile);
    std::vector<Student*> students = StudentsType(studentsList);
    studentsfile.close();

    std::fstream equationsfile = SafeOpen("equations.txt");
    std::vector<std::array<double, 3>> equations = Equations(equationsfile);
    equationsfile.close();

    Professor professor(studentsList);
    professor.checkQueue(equations, students);

    std::fstream gradefile = SafeOpen("gradetable.txt");
    professor.printTable(gradefile);
    gradefile.close();

    for (auto& student : students) 
        delete student;    
}
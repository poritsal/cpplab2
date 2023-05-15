#include "professor.h"

std::array<double, 2> Solution(ESol num, double a, double b, double c)
{
    std::array<double, 2> res{};
    switch (num)
    {
    case ESol::INF:
        res = { INT_MIN, INT_MAX };
        break;
    case ESol::ZERO:
        res = { INT_MAX, INT_MIN };
        break;
    case ESol::ONE_LINEAR:
        res = { (-c) / (b), INT_MIN };
        break;
    case ESol::ONE_PARABOLIC:
        res = { (-b) / (2 * a), INT_MAX };
        break;
    case ESol::TWO:
        if ((-b - sqrt(b * b - 4 * a * c)) / (2 * a) > (-b + sqrt(b * b - 4 * a * c)) / (2 * a)) {
            res = { (-b + sqrt(b * b - 4 * a * c)) / (2 * a), (-b - sqrt(b * b - 4 * a * c)) / (2 * a) };
        }
        else {
            res = { (-b - sqrt(b * b - 4 * a * c)) / (2 * a), (-b + sqrt(b * b - 4 * a * c)) / (2 * a) };
        }
        break;
    }
    return res;
}

std::array<double, 2> LineSol(std::array<double, 3> coef) {
    std::array<double, 2> res{};
    if (coef[1] == 0) {
        if (coef[2] == 0) {
            res = Solution(ESol::INF, coef[0], coef[1], coef[2]);
        }
        else {
            res = Solution(ESol::ZERO, coef[0], coef[1], coef[2]);
        }
    }
    else {
        res = Solution(ESol::ONE_LINEAR, coef[0], coef[1], coef[2]);
    }
    return res;
}

std::array<double, 2> QuadSol(std::array<double, 3> coef) {
    std::array<double, 2> res{};

    if (coef[1] * coef[1] - 4 * coef[0] * coef[2] < 0) {
        res = Solution(ESol::ZERO, coef[0], coef[1], coef[2]);
    }
    else if (coef[1] * coef[1] - 4 * coef[0] * coef[2] > 0) {
        res = Solution(ESol::TWO, coef[0], coef[1], coef[2]);
    }
    else if (coef[1] * coef[1] - 4 * coef[0] * coef[2] == 0) {
        res = Solution(ESol::ONE_PARABOLIC, coef[0], coef[1], coef[2]);
    }

    return res;
}

std::array<double, 2> GoodAns(std::array<double, 3>& coef) {
    std::array<double, 2> res{};

    if (coef[0] == 0) {
        res = LineSol(coef);
    }
    else {
        res = QuadSol(coef);
    }
    return res;
}

std::array<double, 2> BadStudent::solve(std::array<double, 3>& coef)
{
    return { (double)rand(), (double)rand() };
}

std::array<double, 2> NormStudent::solve(std::array<double, 3>& coef)
{
    if (rand() % 2) {
        return { (double)rand(), (double)rand() };
    } else {
        return GoodAns(coef);
    }
}

std::array<double, 2> GoodStudent::solve(std::array<double, 3>& coef)
{
    return GoodAns(coef);
}

void Professor::getAnswer(std::string name, std::array<double, 3> equation, std::array<double, 2> ans)
{
    std::array<double, 2> answer = GoodAns(equation);
    size_t id = (find(students_.begin(), students_.end(), name) - students_.begin());

    progress_[id][1]++;
    if (ans == answer)
        progress_[id][0]++;
}

void Professor::printTable(std::fstream& file)
{
    for (size_t i = 0; i < students_.size(); i++)
        file << students_[i] << " " << progress_[i][0] << " " << progress_[i][1] << std::endl;
}

void Professor::checkQueue(std::vector<std::array<double, 3>> equations, std::vector<Student*> students)
{
    for (auto& equation : equations) {
        for (auto& student : students) {
            getAnswer(student->getName(), equation, student->solve(equation));
        }
    }
}

std::vector<std::string> Students(std::fstream& file) {
    std::vector<std::string> students;
    std::string name;

    while (!file.eof()) {
        std::getline(file, name);
        students.push_back(name);
    }

    return students;
}

std::vector<std::array<double, 3>> Equations(std::fstream& file) {
    std::vector <std::array<double, 3>> res;
    std::array<double, 3> coef{};
    double d = 0;
    while (!file.eof()) {
        for (size_t i = 0; i < 3; i++) {
            file >> d;
            coef[i] = d;
        }
        res.push_back(coef);
    }
    file.close();
    return res;
}

std::vector<Student*> StudentsType(std::vector<std::string> const& studentsNames) {
    std::vector<Student*> students(studentsNames.size());
    for (size_t i = 0; i < studentsNames.size(); i++) {
        int stType = rand() % 3;
        switch (stType)
        {
        case 0:
            students[i] = new BadStudent(studentsNames[i]);
            break;
        case 1:
            students[i] = new NormStudent(studentsNames[i]);
            break;
        case 2:
            students[i] = new GoodStudent(studentsNames[i]);
            break;
        }
    }
    return students;
}

std::fstream SafeOpen(std::string filename) {
    std::fstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cout << "file open error";
        exit(1);
    }
    return file;
}
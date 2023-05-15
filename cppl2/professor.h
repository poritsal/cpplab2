#pragma once
#include "professor.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <array>

enum class ESol {
    ZERO,
    ONE_LINEAR,
    ONE_PARABOLIC,
    TWO,
    INF
};
class Student {
public:
    Student(std::string name) : name_(name) {}
    virtual std::array<double, 2> solve(std::array<double, 3>& coef) = 0;
    std::string getName() { return name_; }
private:
    std::string name_;
};

class BadStudent : public Student {
public:
    BadStudent(std::string name) : Student(name) {}
    std::array<double, 2> solve(std::array<double, 3>& coef) override;
};

class NormStudent : public Student {
public:
    NormStudent(std::string name) : Student(name) {}
    std::array<double, 2> solve(std::array<double, 3>& coef) override;
};

class GoodStudent : public Student {
public:
    GoodStudent(std::string name) : Student(name) {}
    std::array<double, 2> solve(std::array<double, 3>& coef) override;
};


class Professor {
public:
    Professor(std::vector<std::string> students) : students_(students), progress_(students.size(), { 0, 0 }) {}
    void getAnswer(std::string name, std::array<double, 3> equation, std::array<double, 2> ans);
    void printTable(std::fstream& file);
    void checkQueue(std::vector<std::array<double, 3>> equations, std::vector<Student*> students);
private:
    std::vector<std::vector<size_t>> progress_;
    std::vector<std::string> students_;
};

std::array<double, 2> Solution(ESol num, double a, double b, double c);
std::array<double, 2> LineSol(std::array<double, 3> coef);
std::array<double, 2> QuadSol(std::array<double, 3> coef);
std::array<double, 2> GoodAns(std::array<double, 3>& coef);
std::vector<std::string> Students(std::fstream& file);
std::vector<std::array<double, 3>> Equations(std::fstream& file);
std::vector<Student*> StudentsType(std::vector<std::string> const& studentsNames);
std::fstream SafeOpen(std::string filename);
#include <iostream>
#include <string>

class Employee
{
public:
    Employee(const std::string &name) : name(name) {}

    // Pure virtual function for abstraction
    virtual void work() const = 0;

protected:
    std::string name;
};

class Developer : public Employee
{
public:
    Developer(const std::string &name, int age, char sex) : Employee(name), age(age), sex(sex) {}

    // Specific attributes
    int age;
    char sex;

    // Methods
    void writeCode() const
    {
        std::cout << name << " is writing code.\n";
    }

    void debugCode() const
    {
        std::cout << name << " is debugging code.\n";
    }

    void work() const override
    {
        writeCode();
        debugCode();
    }
};

/* class Developer : public Employee {
public:
    // Parameterized constructor
    Developer(const std::string& name, int aage, char asex) : Employee(name) {
        age = aage;
        sex = asex;
    }

    // Specific attributes
    int age;
    char sex; */

class Manager : public Employee
{
public:
    Manager(const std::string &name, int age, char sex) : Employee(name), age(age), sex(sex) {}

    // Specific attributes
    int age;
    char sex;

    // Methods
    void manageTeam() const
    {
        std::cout << name << " is managing the team.\n";
    }

    void setGoals() const
    {
        std::cout << name << " is setting goals.\n";
    }

    void work() const override
    {
        manageTeam();
        setGoals();
    }
};

class Designer : public Employee
{
public:
    Designer(const std::string &name, int age, char sex) : Employee(name), age(age), sex(sex) {}

    // Specific attributes
    int age;
    char sex;

    // Methods
    void createDesigns() const
    {
        std::cout << name << " is creating designs.\n";
    }

    void presentDesigns() const
    {
        std::cout << name << " is presenting designs.\n";
    }

    void work() const override
    {
        createDesigns();
        presentDesigns();
    }
};

class Tester : public Employee
{
public:
    Tester(const std::string &name, int age, char sex) : Employee(name), age(age), sex(sex) {}

    // Specific attributes
    int age;
    char sex;

    // Methods
    void runTests() const
    {
        std::cout << name << " is running tests.\n";
    }

    void reportBugs() const
    {
        std::cout << name << " is reporting bugs.\n";
    }

    void work() const override
    {
        runTests();
        reportBugs();
    }
};

class Architect : public Employee
{
public:
    Architect(const std::string &name, int age, char sex) : Employee(name), age(age), sex(sex) {}

    // Specific attributes
    int age;
    char sex;

    // Methods
    void planArchitecture() const
    {
        std::cout << name << " is planning architecture.\n";
    }

    void reviewDesigns() const
    {
        std::cout << name << " is reviewing designs.\n";
    }

    void work() const override
    {
        planArchitecture();
        reviewDesigns();
        }
};

int main()
{
    // Using polymorphism with a pointer to the base class
    Employee *employees[] = {
        new Developer("Alice", 25, 'F'),
        new Manager("Bob", 35, 'M'),
        new Designer("Charlie", 28, 'F'),
        new Tester("David", 30, 'M'),
        new Architect("Eve", 40, 'F')};

    // Call the work() function for each employee
    for (const auto &employee : employees)
    {
        employee->work();
    }

    // Clean up memory
    for (const auto &employee : employees)
    {
        delete employee;
    }

    return 0;
}

/*
for (int i = 0; i < n; ++i) {
        employees[i]->work();
    }

    // Clean up memory
    for (int i = 0; i < n; ++i) {
        delete employees[i];
    }

    return 0;
} */

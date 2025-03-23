#include <iostream>
#include <string>

using namespace std;

class Bank
{
public:
    Bank(const string &personName, int accountNumber, double balance)
        : personName(personName), accountNumber(accountNumber), balance(balance) {}

    // Pure virtual function for displaying bank information
    virtual void displayInfo() const = 0;

    // Virtual destructor (important when using polymorphism)
    virtual ~Bank() {}

    // Common methods for all banks
    void deposit(double amount)
    {
        balance += amount;
        cout << "Deposit of $" << amount << " successful. New balance: $" << balance << "\n";
    }

    void withdraw(double amount)
    {
        if (amount > balance)
        {
            cout << "Insufficient funds. Withdrawal failed.\n";
        }
        else
        {
            balance -= amount;
            cout << "Withdrawal of $" << amount << " successful. New balance: $" << balance << "\n";
        }
    }

    virtual void makeLoan(double amount) = 0;

protected:
    string personName;
    int accountNumber;
    double balance;
};

class BankOfTunisia : public Bank
{
public:
    BankOfTunisia(const string &personName, int accountNumber, double balance, int creditScore)
        : Bank(personName, accountNumber, balance), creditScore(creditScore) {}

    // Override the displayInfo function to provide specific information for Bank of Tunisia
    void displayInfo() const override
    {
        cout << "Bank of Tunisia - Person: " << personName << ", Account Number: " << accountNumber
             << ", Balance: $" << balance << ", Credit Score: " << creditScore << "\n";
    }

    // Override the makeLoan function to provide specific loan logic for Bank of Tunisia
    void makeLoan(double amount) override
    {
        if (amount <= balance * 0.5 && creditScore > 700)
        {
            balance += amount;
            cout << "Loan of $" << amount << " approved. New balance: $" << balance << "\n";
        }
        else
        {
            cout << "Loan request denied. Insufficient credit score or too high loan amount.\n";
        }
    }

private:
    int creditScore;
};

class BankOfAlgeria : public Bank
{
public:
    BankOfAlgeria(const string &personName, int accountNumber, double balance, double monthlySalary)
        : Bank(personName, accountNumber, balance), monthlySalary(monthlySalary) {}

    // Override the displayInfo function to provide specific information for Bank of Algeria
    void displayInfo() const override
    {
        cout << "Bank of Algeria - Person: " << personName << ", Account Number: " << accountNumber
             << ", Balance: $" << balance << ", Monthly Salary: $" << monthlySalary << "\n";
    }

    // Override the makeLoan function to provide specific loan logic for Bank of Algeria
    void makeLoan(double amount) override
    {
        if (amount <= monthlySalary * 3)
        {
            balance += amount;
            cout << "Loan of $" << amount << " approved. New balance: $" << balance << "\n";
        }
        else
        {
            cout << "Loan request denied. Loan amount exceeds three times the monthly salary.\n";
        }
    }

private:
    double monthlySalary;
};

int main()
{
    // Get the number of persons to manipulate
    int numPersons;
    cout << "Enter the number of persons to manipulate: ";
    cin >> numPersons;

    // Create an array to hold pointers to Bank objects
    Bank *banks[numPersons];

    // Get information for each person
    for (int i = 0; i < numPersons; ++i)
    {
        string personName;
        int accountNumber;
        double balance;

        cout << "Enter person name for person " << i + 1 << ": ";
        cin >> personName;

        cout << "Enter account number for person " << i + 1 << ": ";
        cin >> accountNumber;

        cout << "Enter balance for person " << i + 1 << ": $";
        cin >> balance;

        // Get additional information based on the chosen bank type
        char bankType;
        cout << "Choose bank type for person " << i + 1 << " (T for Tunisia, A for Algeria): ";
        cin >> bankType;

        if (bankType == 'T' || bankType == 't')
        {
            int creditScore;
            cout << "Enter credit score for person " << i + 1 << ": ";
            cin >> creditScore;

            banks[i] = new BankOfTunisia(personName, accountNumber, balance, creditScore);
        }
        else if (bankType == 'A' || bankType == 'a')
        {
            double monthlySalary;
            cout << "Enter monthly salary for person " << i + 1 << ": $";
            cin >> monthlySalary;

            banks[i] = new BankOfAlgeria(personName, accountNumber, balance, monthlySalary);
        }
        else
        {
            cout << "Invalid bank type. Exiting program.\n";
            return 1;
        }
    }
    cout << "---------------------------------\n\n\n\n";
    // Perform operations for each person
    for (int i = 0; i < numPersons; ++i)
    {
        double depositAmount, withdrawAmount, loanAmount;

        cout << "Enter deposit amount for person " << i + 1 << ": $";
        cin >> depositAmount;
        banks[i]->deposit(depositAmount);

        cout << "Enter withdrawal amount for person " << i + 1 << ": $";
        cin >> withdrawAmount;
        banks[i]->withdraw(withdrawAmount);

        cout << "Enter loan amount for person " << i + 1 << ": $";
        cin >> loanAmount;
        banks[i]->makeLoan(loanAmount);

        banks[i]->displayInfo();
        cout << "---------------------------------\n";
    }

    // Clean up memory
    for (int i = 0; i < numPersons; ++i)
    {
        delete banks[i];
    }

    return 0;
}

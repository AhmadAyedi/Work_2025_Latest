#include <iostream>
using namespace std;

int main()
{
    int number = 10;
    int &ref = number; // ref is a reference to number

    cout << "Original number: " << number << endl;
    cout << "Reference: " << ref << endl;

    ref = 20; // Changing ref changes number because they are the same!
    cout << "After modifying reference: " << number << endl;

    return 0;
}

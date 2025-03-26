#include <iostream>
using namespace std;

int main()
{
    int number = 10;
    int *ptr = &number; // ptr stores the address of number

    cout << "Value of number: " << number << endl;
    cout << "Address of number: " << &number << endl;
    cout << "Pointer stores: " << ptr << endl;
    cout << "Value at pointer: " << *ptr << endl; // Dereferencing the pointer

    *ptr = 20; // Changing value at pointer also changes number
    cout << "After modifying pointer: " << number << endl;

    return 0;
}

#include <stdio.h>

struct Person
{
    char name[50];
    int age;
};

void updatePerson(struct Person *p)
{
    p->age = 30; // Directly modifies the original struct through the pointer.
    printf("Updated age: %d\n", p->age);
}

int main()
{
    struct Person person1 = {"Alice", 25};
    updatePerson(&person1); // Passes the pointer to the struct (no copying).
    return 0;
}

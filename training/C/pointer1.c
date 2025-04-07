#include <stdio.h>
#include <stdlib.h>

void stackMemoryExample()
{
    int a = 10; // stack variable
    printf("Stack Variable a: %d, Address: %p\n", a, (void *)&a);
}

void heapMemoryExample()
{
    int *arr = (int *)malloc(5 * sizeof(int)); // heap memory allocation

    if (arr == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    // Populate and print the array
    for (int i = 0; i < 5; i++)
    {
        arr[i] = i * 10;
        printf("Heap arr[%d] = %d, Address: %p\n", i, arr[i], (void *)&arr[i]);
    }

    // Don't forget to free the heap memory
    free(arr);
}

int main()
{
    stackMemoryExample(); // stack usage
    heapMemoryExample();  // heap usage
    return 0;
}

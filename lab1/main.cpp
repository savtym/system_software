#include <iostream>
#include "stdio.h"
#include "Allocator.h"

using namespace std;

int main() {

    printf("Work Started!\n");

    Allocator al = Allocator(3000);
    al.Print();

    printf("********************************\n");
    printf("********************************\n");

    void* add1 = al.mem_alloc(1000);
    void* add2 = al.mem_alloc(500);
    void* add3 = al.mem_alloc(500);


    printf("After first allocation.\n");
    al.Print();

    printf("********************************\n");
    printf("********************************\n");

    al.mem_free(add2);
    printf("After free.\n");
    al.Print();

    printf("********************************\n");
    printf("********************************\n");

    al.mem_realloc(add1, 500);
    printf("After first reallocation.\n");
    al.Print();

    printf("********************************\n");
    printf("********************************\n");

    printf("After second reallocation.\n");
    al.mem_realloc(add3, 1000);
    al.Print();

	std::getchar();
    return 0;
}
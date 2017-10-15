#include <iostream>

#include "stdio.h"

#include "pagememoryallocator.h"

using namespace std;

int main() {

	printf("Work Started!\n");

	PageMemoryAllocator al = PageMemoryAllocator();
	cout << al.mem_dump() << "\n\n";

	printf("********************************\n");
	printf("********************************\n\n");

	void* add1 = al.mem_alloc(2000);
	void* add2 = al.mem_alloc(1000);
	void* add3 = al.mem_alloc(1000);


	printf("After first allocation.\n");
	cout << al.mem_dump() << "\n\n";

	printf("********************************\n");
	printf("********************************\n\n");

	al.mem_free(add2);
	printf("After free.\n");
	cout << al.mem_dump() << "\n\n";

	printf("********************************\n");
	printf("********************************\n\n");

	al.mem_realloc(add1, 500);
	printf("After first reallocation.\n");
	cout << al.mem_dump() << "\n\n";

	printf("********************************\n");
	printf("********************************\n\n");

	printf("After second reallocation.\n");
	al.mem_realloc(add3, 1000);
	cout << al.mem_dump() << "\n";

	std::getchar();
	return 0;
}
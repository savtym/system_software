
/*Звідси беремо константи для налаштувань за замовченням.*/
#include "constants.h"

/*Звідси беремо клас "MemoryPage".*/
#include "memorypage.h"

/*Звідси беремо команду "malloc".*/
#include <stdlib.h>

/*Звідси беремо контейнер "vector".*/
#include <vector>

#include <unordered_map>
#include <string>

using namespace std;

/*Аллокатор пам'яті, який діє за сторінковим принципом.*/
class PageMemoryAllocator {

public:
    /*Конструктор за замовченням.*/
    PageMemoryAllocator();

    /*Конструктор з параметрами.*/
    PageMemoryAllocator(unsigned int newMemorySize, unsigned int newPageSize);

    /*
     * Спроба виділити пам'ять заданого розміру.
     * При успіху повертає адресу початку виділеної області пам'яті.
     * Інакше повертає NULL.
     * */
    void* mem_alloc(size_t size);

    /*Спроба звільнити блок пам'яті із заданою адресою початку.*/
    void mem_free(void* addr);

    /*Спроба змінити розмір блоку пам'яті із заданою адресою початку.*/
    void *mem_realloc(void* addr, size_t newSize);

    /*Повертає текстову інформацію про поточний стан буферної пам'яті.*/
    string mem_dump();

private:

    /*Спроба розділити наявну пам'ять між сторінками.*/
    void ShareMemoryBetweenPages();

    /*Обчислення можливих розмірів блоків.*/
    void CalculatePossibleBlockSizes();

    /*Ініціалізація векторів номерів сторінок.*/
    void PageNumberVectorsInitializtion();

    /*Спроба визначити клас блоку, який потрібний для розміщення пам'яті заданого розміру.*/
    unsigned int FindBlockClassWithRequiredSize(size_t size);

    /*
     * Спроба виділити блок пам'яті, що складається з визначеної кількості сторінок.
     * В разі успіху повертає true.
     * Інакше повертає false.
     * */
    void* AllocateBigMemoryBlock(unsigned int countOfPages);

    /*Визначає, чи потрапляє у буферну пам'ять задана адреса.*/
    bool IsInMemoryRange(void* addr);

    /*Буферна пам'ять.*/
    void* memory;

    /*Загальний розмір буферної пам'яті у байтах.*/
    unsigned int memorySize;

    /*Розмір однієї сторінки у байтах.*/
    unsigned int pageSize;

    /*Максимальний допустимий розмір для половини сторінки.*/
    unsigned int maxBase;

    /*Вектор описів сторінок пам'яті.*/
    vector<MemoryPage> memoryPages;

    /*Вектор номерів вільних сторінок.*/
    vector<unsigned int> freePageNumbers;

    /*Вектор номерів частково зайнятих сторінок, розбитих за класами.*/
    vector< vector<unsigned int> > inUsePageNumbers;

    /*Вектор номерів повністю зайнятих сторінок.*/
    vector<unsigned int> fullFilledPageNumbers;

    /*Вектор можливих розмірів блоку, який можна умістити на одну сторінку.*/
    vector<unsigned int> possibleBlockSizes;

    /*Хеш-таблиця блоків, що займають одну або більше сторінок пам'яті.*/
    unordered_map<void*, vector<unsigned int> > bigBlocks;
};


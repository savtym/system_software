
#include "stdio.h"
#include <algorithm>
#include<vector>

using namespace std;


/*Сторінка пам'яті.*/
class MemoryPage {
public:
    /*Конструктор за замовченням.*/
    MemoryPage();

    /*Конструктор з параметрами.*/
    MemoryPage(void* startAddress);

    /*Спроба розбити сторінку на блоки однакового розміру.*/
    void SplitPage(unsigned int blockSize, unsigned int countOfBlocks);

    /*
     * Спроба виділити перший вільний блок.
     * В разі успіху повертає адресу початку цього блоку.
     * В іншому випадку повертає NULL.
     * */
    void* AllocateFirstFreeBlock();

    /*
     * Спроба звільнити блок із заданою адресою.
     * */
    void FreeBlock(void* address);

    /*
     * Перевірка на заповненість сторінки.
     * */
    bool IsFullFilled();

    /*
     * Перевірка порожність сторінки.
     * */
    bool IsEmpty();

    void *GetMemory() const;

private:

    /*Пам'ять сторінки.*/
    void* memory;

    /*Вільні блоки.*/
    vector<void*> freeBlockAddresses;

    /*Зайняті блоки.*/
    vector<void*> blocksInUseAddresses;
};


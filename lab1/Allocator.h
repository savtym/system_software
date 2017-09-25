#include "malloc.h"
#include "Block.h"
#include "string.h"
#include "cstring"
#include <vector>

using namespace std;

/*Клас-аллокатор пам'яті.*/
class Allocator {
public:
    /*Конструктор за замовченням.*/
    Allocator();

    /*Параметризований конструктор.*/
    Allocator(const unsigned int newMemorySize);

    /*Деструктор.*/
    ~Allocator();

    /*Спроба виділеня області пам'яті заданого розміру.*/
    void* mem_alloc(size_t size);

    /*Спроба звільнити задану область пам'яті.*/
    void mem_free(void* addr);

    /*Спроба змінити розмір блока з визначеною адресою.*/
    void* mem_realloc(void* addr, size_t size);

    /*Виводить у консоль інформвцію про блок.*/
    void Print() {
        printf("\nMemory Size: %d\nCount of free blocks: %d\n", memorySize, (int)freeBlocks.size());
        for (int i = 0; i < freeBlocks.size(); i++) {
            printf("\nCurrent block: %d", i + 1);
            freeBlocks[i].Print();
        }

        printf("\nCount of allocated blocks: %d\n", (int)allocatedBlocks.size());
        for (int i = 0; i < allocatedBlocks.size(); i++) {
            printf("\nCurrent block: %d", i + 1);
            allocatedBlocks[i].Print();
        }
    }

private:
    /*Початкова ініціалізація змінних.*/
    void VarsInitialization(unsigned int newMemorySize);

    /*Початкове виділення пам'яті.*/
    void StartingMemoryAllocation();
    
    /*Спроба склеювання сусідніх вільних блоків пам'яті.*/
    void MergeFreeWith(Block block);

    /*Розмір пулу пам'яті у байтах.*/
    unsigned int memorySize;

    /*Власне пул.*/
    void* memory;

    /*Заголовки порожніх блоків.*/
    vector<Block> freeBlocks;

    /*Заголовки зайнятих блоків.*/
    vector<Block> allocatedBlocks;
};

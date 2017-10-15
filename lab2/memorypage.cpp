#include "memorypage.h"

MemoryPage::MemoryPage() {
    memory = NULL;
    freeBlockAddresses = vector<void*>();
    blocksInUseAddresses = vector<void*>();
}

/*Конструктор з параметрами.*/
MemoryPage::MemoryPage(void *startAddress) {
    memory = startAddress;
    freeBlockAddresses = vector<void*>();
    blocksInUseAddresses = vector<void*>();
}

/*Спроба розбити сторінку на блоки однакового розміру.*/
void MemoryPage::SplitPage(unsigned int blockSize, unsigned int countOfBlocks) {
    freeBlockAddresses.clear();
    blocksInUseAddresses.clear();

    for(unsigned int i = 0; i < countOfBlocks; i++){
        freeBlockAddresses.push_back((void *)((unsigned int)memory + blockSize * i));
    }
}

/*
 * Спроба виділити перший вільний блок.
 * В разі успіху повертає адресу початку цього блоку.
 * В іншому випадку повертає NULL.
 * */
void *MemoryPage::AllocateFirstFreeBlock() {
    if(freeBlockAddresses.empty()){
        return NULL;
    }else{
        void* ret = freeBlockAddresses[0];
        freeBlockAddresses.erase(freeBlockAddresses.begin());
        blocksInUseAddresses.push_back(ret);
        return ret;
    }
}

/*
 * Спроба звільнити блок із заданою адресою.
 * */
void MemoryPage::FreeBlock(void *address) {
    vector<void*>::iterator it = find(blocksInUseAddresses.begin(), blocksInUseAddresses.end(), address);

    if(blocksInUseAddresses.end() == it){
        return;
    }

    freeBlockAddresses.push_back(*it);
    sort(freeBlockAddresses.begin(), freeBlockAddresses.end());
    blocksInUseAddresses.erase(it);
}

/*
 * Перевірка на заповненість сторінки.
 * */
bool MemoryPage::IsFullFilled() {
    return freeBlockAddresses.empty();
}

/*
 * Перевірка порожність сторінки.
 * */
bool MemoryPage::IsEmpty() {
    return blocksInUseAddresses.empty();
}

void *MemoryPage::GetMemory() const {
    return memory;
}


#include <iostream>
#include "malloc.h"
#include "stdio.h"


/*Містить інформацію про блок пам'яті.*/
class Block {
public:
    /*Конструктор за замовченням.*/
    Block();

    /*Параметризований конструктор.*/
    Block(unsigned int newSize, void* newStartAddress);

    /*Деструктор.*/
    ~Block();

    /*Спроба виділити блок заданого розміру із даного блоку.*/
    Block* Allocate(unsigned int sizeToAllocate);

    /*Встановлення нового розміру блоку.*/
    void SetSize(unsigned int newSize){
        size = newSize;
    }

    /*Повертає поточний розмір блоку.*/
    unsigned int Size(){
        return size;
    }

    /*Повертає початкову адресу блоку.*/
    void* StartAddress(){
        return startAddress;
    }

    /*Виводить у консоль інформацію про блок.*/
    void Print(){
        printf("\nSize: %d\tAddr %d\n", size, startAddress);
    }

private:

    /*Повертає TRUE у випадку, якщо з цього блоку можна виділити визначену кількість пам'яті.*/
    bool CanAllocate(unsigned int sizeToAllocate){
        return sizeToAllocate <= size;
    }

    /*Розмір блоку.*/
    unsigned int size;

    /* праворець */
    bool isFree;

    /*Адреса початку блоку.*/
    void* startAddress;
};

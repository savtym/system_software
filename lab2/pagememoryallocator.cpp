#include "pagememoryallocator.h"


/*Конструктор за замовченням.*/
PageMemoryAllocator::PageMemoryAllocator() {
    pageSize =  Constants::DEFAULT_PAGE_SIZE;
    memorySize = Constants::DEFAULT_MEMORY_SIZE;
    memory = (void*)malloc(memorySize * 4);

    possibleBlockSizes = vector<unsigned int>();

    CalculatePossibleBlockSizes();
    PageNumberVectorsInitializtion();
    ShareMemoryBetweenPages();

    bigBlocks = unordered_map<void*, vector<unsigned int> >();
}

/*Конструктор з параметрами.*/
PageMemoryAllocator::PageMemoryAllocator(unsigned int newMemorySize, unsigned int newPageSize) {
    pageSize = newPageSize;
    memorySize = newMemorySize;
    memory = (void*)malloc(memorySize * 4);

    possibleBlockSizes = vector<unsigned int>();

    CalculatePossibleBlockSizes();
    PageNumberVectorsInitializtion();
    ShareMemoryBetweenPages();

    bigBlocks = unordered_map<void*, vector<unsigned int> >();
}

/*
 * Спроба виділити пам'ять заданого розміру.
 * При успіху повертає адресу початку виділеної області пам'яті.
 * Інакше повертає NULL.
 * */
void *PageMemoryAllocator::mem_alloc(size_t size) {
    void* ret = NULL;

    /*Якщо необхідний розмір блоку менший за половину сторінки, то:...*/
    if (size <= pageSize / 2) {

        /*1. З'ясовується клас цього блоку*/
        unsigned int requiredBlockClass = FindBlockClassWithRequiredSize(size);

        /*2. Виконується пошук сторінки, яка містить вільний блок необхідного розміру.*/
        /*Якщо сторінок, поділених на блоки відповідного розміру немає, то:...*/
        if (inUsePageNumbers[requiredBlockClass].empty()) {

            /*1. Якщо вільних сторінок немає, повертається NULL.*/
            if (freePageNumbers.empty()) {
                return ret;
            }

            /*2. Інакше береться перша вільна сторінка, ділиться на блоки, перший з цих блоків повертається, а сама сторінка переходить в розряд тих, що використовуються.*/
            memoryPages[freePageNumbers[0]].SplitPage(possibleBlockSizes[requiredBlockClass], pageSize / possibleBlockSizes[requiredBlockClass]);
            ret = memoryPages[freePageNumbers[0]].AllocateFirstFreeBlock();
            inUsePageNumbers[requiredBlockClass].push_back(freePageNumbers[0]);
            freePageNumbers.erase(freePageNumbers.begin());

        }else{

            /*Якщо необхідні сторінки є, то у першій з них виділяється перший доступний блок.*/
            ret = memoryPages[inUsePageNumbers[requiredBlockClass][0]].AllocateFirstFreeBlock();

            /*Якщо після цього сторінка виявиться заповненою, про це окремо вказується.*/
            if(memoryPages[inUsePageNumbers[requiredBlockClass][0]].IsFullFilled()){
                fullFilledPageNumbers.push_back(inUsePageNumbers[requiredBlockClass][0]);
                inUsePageNumbers[requiredBlockClass].erase(inUsePageNumbers[requiredBlockClass].begin());
            }
        }
    }else{

        /*Якщо необхідний блок, більший за половину сторінки, то:...*/
        unsigned int requiredPagesCount = size / pageSize + 1;
        if(size % pageSize == 0){
            requiredPagesCount--;
        }

        if(requiredPagesCount > freePageNumbers.size()){
            return ret;
        }

        return AllocateBigMemoryBlock(requiredPagesCount);
    }

    return ret;
}

/*Спроба звільнити блок пам'яті із заданою адресою початку.*/
void PageMemoryAllocator::mem_free(void *addr) {
    if(!IsInMemoryRange(addr)){
        return;
    }

    /*Чи не належить даний блок до класу великих блоків?*/
    unordered_map<void*, vector<unsigned int> >::const_iterator gotInfo = bigBlocks.find(addr);
    if(gotInfo == bigBlocks.end()){
        /*Визначення сторінки, до якої відноситься даний блок.*/
        unsigned int pageNumber = ((unsigned int)addr - (unsigned int)memory) / pageSize;

        memoryPages[pageNumber].FreeBlock(addr);

        if(memoryPages[pageNumber].IsEmpty()){
            for(unsigned int i = 0; i < inUsePageNumbers.size(); i++){
                vector<unsigned int>::const_iterator it = find(inUsePageNumbers[i].begin(), inUsePageNumbers[i].end(), pageNumber);

                if(it != inUsePageNumbers[i].end()){
                    freePageNumbers.push_back(*it);
                    sort(freePageNumbers.begin(), freePageNumbers.end());
                    inUsePageNumbers[i].erase(it);
                    break;
                }
            }
        }
    }else{
        for(unsigned int i = 0; i < gotInfo->second.size(); i++){
            freePageNumbers.push_back(gotInfo->second[i]);
            fullFilledPageNumbers.erase(find(fullFilledPageNumbers.begin(), fullFilledPageNumbers.end(), gotInfo->second[i]));
        }
        bigBlocks.erase(gotInfo);
        sort(freePageNumbers.begin(), freePageNumbers.end());
    }

}

/*Спроба змінити розмір блоку пам'яті із заданою адресою початку.*/
void *PageMemoryAllocator::mem_realloc(void *addr, size_t newSize) {
    if(!IsInMemoryRange(addr)){
        return NULL;
    }

    if(newSize <= 0){
        mem_free(addr);
        return NULL;
    }

    /*Чи не належить даний блок до класу великих блоків?*/
    unordered_map<void*, vector<unsigned int> >::const_iterator gotInfo = bigBlocks.find(addr);
    if(gotInfo == bigBlocks.end()){
        /*Визначення сторінки, до якої відноситься даний блок.*/
        unsigned int pageNumber = ((unsigned int)addr - (unsigned int)memory) / pageSize;

        /*Визначення класу блоку.*/
        for (unsigned int i = 0; i < inUsePageNumbers.size(); i++) {
            vector<unsigned int>::const_iterator it = find(inUsePageNumbers[i].begin(), inUsePageNumbers[i].end(), pageNumber);

            if (it != inUsePageNumbers[i].end()) {
                /*Визначення номеру блоку, до якого відноситься адреса.*/
                unsigned int blockNumber = ((unsigned int)addr - (unsigned int)(memoryPages[pageNumber]).GetMemory()) / possibleBlockSizes[i];

                if ((unsigned int)memoryPages[pageNumber].GetMemory() + blockNumber * possibleBlockSizes[i] != (unsigned int)addr) {
                    return NULL;
                }

                void* buf = (void*)malloc(possibleBlockSizes[i]);
                memcpy(buf, addr, possibleBlockSizes[i]);
                mem_free(addr);
                void* ret = mem_alloc(newSize);

                /*Якщо місця для нового блоку немає, то потрібно повернути все назад.*/
                if(ret == NULL){
                    ret = mem_alloc(possibleBlockSizes[i]);
                    memcpy(ret, addr, possibleBlockSizes[i]);
                }else{
                    memcpy(ret, addr, newSize);
                }

                free(buf);
                return ret;
            }
        }
        return NULL;

    }else{
        /*Чи ми зменшуємо або не змінюємо розмір блоку?*/
        unsigned int oldSize = gotInfo->second.size() * pageSize;
        if(oldSize >= newSize){
            /*Якщо ми зменшуємо великий блок, то потрібно звільнити визначену кількість сторінок в його кінці.*/
            unsigned int countOfPagesToFree;
            if(newSize <= pageSize / 2){
                countOfPagesToFree = gotInfo->second.size();
            }else{
                countOfPagesToFree = (oldSize - newSize) / pageSize;
            }
            vector<unsigned int> v = vector<unsigned int>(gotInfo->second);

            for(unsigned int i = 0; i < countOfPagesToFree; i++){
                freePageNumbers.push_back(v.back());
                fullFilledPageNumbers.erase(find(fullFilledPageNumbers.begin(), fullFilledPageNumbers.end(), v.back()));
                v.pop_back();
            }

            bigBlocks.erase(gotInfo);
            bigBlocks.emplace(addr, v);
            sort(freePageNumbers.begin(), freePageNumbers.end());

            /*Якщо блок після цього переходить в розряд маленьких, то запис про нього видаляється.*/
            if(gotInfo->second.empty()){
                bigBlocks.erase(gotInfo);

                void* buf = (void*)malloc(newSize);
                memcpy(buf, addr, newSize);

                void* ret = mem_alloc(newSize);
                memcpy(ret, buf, newSize);

                free(buf);
                return ret;
            }

            /*Якщо блок залишається великим, то нічого більше не змінюється.*/
            return addr;
        }
        /*Якщо ми збільшуємо блок, то потрібно спочатку очистити поточний, запам'ятавши його вміст.*/
        void* buf = (void*)malloc(oldSize);
        memcpy(buf, addr, oldSize);
        mem_free(addr);
        void* ret = mem_alloc(newSize);

        /*Якщо місця для нового блоку немає, то потрібно повернути все назад.*/
        if(ret == NULL){
            ret = mem_alloc(oldSize);
            memcpy(ret, addr, oldSize);
        }else{
            memcpy(ret, addr, newSize);
        }

        free(buf);
        return ret;
    }

    return NULL;
}

string PageMemoryAllocator::mem_dump() {
    string ret = "";

    ret.append("Memory total (bytes): ");
    ret.append(to_string(memorySize));
    ret.append("\nPage size (bytes): ");
    ret.append(to_string(pageSize));

    ret.append("\nCount of pages: ");
    ret.append(to_string(memoryPages.size()));

    ret.append("\nOf them:");

    ret.append("\n\tis free: ");
    ret.append(to_string(freePageNumbers.size()));

    ret.append("\n\tnot completely full: ");
    unsigned int inUseCount = 0;
    for(unsigned int i = 0; i < inUsePageNumbers.size(); i++){
        for(unsigned int j = 0; j < inUsePageNumbers[i].size(); j++){
            inUseCount++;
        }
    }
    ret.append(to_string(inUseCount));

    ret.append("\n\tFully filled: ");
    ret.append(to_string(fullFilledPageNumbers.size()));

    return ret;
}

/*Спроба розділити наявну пам'ять між сторінками.*/
void PageMemoryAllocator::ShareMemoryBetweenPages() {
    unsigned int pageCount = memorySize / pageSize;
    for(unsigned int i = 0; i < pageCount; i++){
        memoryPages.push_back(MemoryPage((void *)((unsigned int)memory + i * pageSize)));
        freePageNumbers.push_back(i);
    }
}

/*Обчислення можливих розмірів блоків.*/
void PageMemoryAllocator::CalculatePossibleBlockSizes() {
    possibleBlockSizes.push_back(1);
    unsigned int base = 2;

    do{
        possibleBlockSizes.push_back(base);

        base = base << 1;
    }while(base <= pageSize / 2);

    maxBase = base;
}

/*Ініціалізація векторів номерів сторінок.*/
void PageMemoryAllocator::PageNumberVectorsInitializtion() {
    freePageNumbers = vector<unsigned int>();
    inUsePageNumbers =  vector< vector<unsigned int> >();

    for(unsigned int i = 0; i < possibleBlockSizes.size(); i++){
        inUsePageNumbers.push_back(vector<unsigned int>());
    }

    fullFilledPageNumbers = vector<unsigned int>();
}

/*Спроба визначити клас блоку, який потрібний для розміщення пам'яті заданого розміру.*/
unsigned int PageMemoryAllocator::FindBlockClassWithRequiredSize(size_t size) {
    for(unsigned int i = 0; i < possibleBlockSizes.size(); i++){
        if(size <= possibleBlockSizes[i]){
            return i;
        }
    }
    return -1;
}

/*
 * Спроба виділити блок пам'яті, що складається з визначеної кількості сторінок.
 * В разі успіху повертає true.
 * Інакше повертає false.
 * */
void* PageMemoryAllocator::AllocateBigMemoryBlock(unsigned int countOfPages) {

    void* ret = NULL;
    for(unsigned int i = 0; i <= freePageNumbers.size() - countOfPages;){
        bool blockIsFound = true;
        for(unsigned int j = 1; j < countOfPages; j++){
            if(freePageNumbers[i + j] != freePageNumbers[i] + j){
                i += j;
                blockIsFound = false;
                break;
            }
        }

        if(blockIsFound){
            vector<unsigned int> pageNumbers = vector<unsigned int>();

            for(unsigned int j = i; j < i + countOfPages; j++){
                fullFilledPageNumbers.push_back(freePageNumbers[j]);

                pageNumbers.push_back(freePageNumbers[j]);
            }

            ret = memoryPages[freePageNumbers[i]].GetMemory();
            bigBlocks.emplace(ret, pageNumbers);
            freePageNumbers.erase(freePageNumbers.begin() + i, freePageNumbers.begin() + countOfPages);
            break;
        }
    }

    return ret;
}

bool PageMemoryAllocator::IsInMemoryRange(void *addr) {
    return NULL != addr && addr >= memory && addr <= (void *)((unsigned int)memory + memorySize);
}


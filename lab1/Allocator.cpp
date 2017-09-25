
#include "Allocator.h"

Allocator::Allocator() {
    VarsInitialization(0);
}

Allocator::Allocator(const unsigned int newMemorySize){
    VarsInitialization(newMemorySize);
    StartingMemoryAllocation();
    freeBlocks.push_back(Block(memorySize, memory));
}

Allocator::~Allocator(){
    freeBlocks.clear();
    allocatedBlocks.clear();
}

/*Спроба виділення пам'яті заданого розміру.*/
void* Allocator::mem_alloc(size_t size){
    /*Область пам'яті можна виділити, якщо її розмір не перевищує розміру доступної пам'яті.*/
    if(size > memorySize){
        return NULL;
    }

    /*Область для виділення слід шукати серед вільних блоків.*/
    for(int i = 0; i < freeBlocks.size(); i++) {
        /*Потрібно знайти перший доступний за розміром блок.*/
        if(freeBlocks[i].Size() >= size){
            /*Якщо його розмір співпадає із потрібним, цей блок просто заноситься у список зайнятих.*/
            if(size == freeBlocks[i].Size()){
                allocatedBlocks.push_back(freeBlocks[i]);
                freeBlocks.erase(freeBlocks.begin() + i);
                return allocatedBlocks.end()->StartAddress();
            }

            /*Інакше з нього виділяється блок потрібного розміру і заноситься у список зайнятих.*/
            Block allocatedBlock = *freeBlocks[i].Allocate(size);
            allocatedBlocks.push_back(allocatedBlock);
            return allocatedBlock.StartAddress();
        }
    }
    /*Якщо блоків потрібного роміру не знайшлось, про це сигналізується.*/
    return NULL;
}

/*Спроба звільнити задану область пам'яті.*/
void Allocator::mem_free(void *addr){
    /*Блок для звільнення шукається серед зайнятих.*/
    for(int i = 0; i < allocatedBlocks.size(); i++){
        if(allocatedBlocks[i].StartAddress() == addr){
            /*Якщо потрібний блок знайдено, він звільняється і склеюється із сусідніми вільними.*/
            MergeFreeWith(allocatedBlocks[i]);
            allocatedBlocks.erase(allocatedBlocks.begin() + i);
            return;
        }
    }
}

/*Спроба злиття поточного вільного блоку із вільними лівішим і правішим (якщо такі є).*/
void Allocator::MergeFreeWith(Block block){
    Block temp = block;
    bool onceMerged = false;

    /*Злиття із лівішим блоком.*/
    for(int i = 0; i < freeBlocks.size(); i++){
        if((unsigned int *)freeBlocks[i].StartAddress() + freeBlocks[i].Size()  == (unsigned int *)temp.StartAddress()){
            freeBlocks[i].SetSize(freeBlocks[i].Size() + temp.Size());
            temp.~Block();
            temp = freeBlocks[i];
            onceMerged = true;
            break;
        }
    }

    /*Злиття із правішим блоком.*/
    for(int i = 0; i < freeBlocks.size(); i++){
        if((unsigned int *)temp.StartAddress() + temp.Size() == (unsigned int *)freeBlocks[i].StartAddress()){
            temp.SetSize(temp.Size() + freeBlocks[i].Size());
            freeBlocks.erase(freeBlocks.begin() + i);
            freeBlocks.push_back(temp);
            return;
        }
    }

    if(!onceMerged){
        freeBlocks.push_back(block);
    }
}

/*Спроба змінити розмір блоку з заданою початковою адресою.*/
void* Allocator::mem_realloc(void *addr, size_t size){
    for(int i = 0; i < allocatedBlocks.size(); i++){
        if(allocatedBlocks[i].StartAddress() == addr){

            /*Якщо новий розмір співпадає із попереднім, то робити більше нічого.*/
            if(size == allocatedBlocks[i].Size()){
                return addr;
            }

            void* possibleDestination = mem_alloc(size);
            if(NULL != possibleDestination){
				try {
					if (size <= allocatedBlocks[i].Size()) {
						memcpy(possibleDestination, addr, size);
					}
					else {
						memcpy(possibleDestination, addr, allocatedBlocks[i].Size());
					}
				}
				catch (const char* message) {
					cout << message << endl;
				}
                mem_free(addr);
                return possibleDestination;
            }
        }
    }
    return addr;
}

void Allocator::VarsInitialization(unsigned int newMemorySize){
    memory = NULL;
    memorySize = newMemorySize;
    freeBlocks = vector<Block>();
    allocatedBlocks = vector<Block>();
}

void Allocator::StartingMemoryAllocation(){
    if(NULL == memory) {
        memory = (void *) malloc(memorySize * 4);
    }
}

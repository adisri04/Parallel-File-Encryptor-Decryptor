#include <iostream>
#include "ProcessManagement.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include "../encryptDecrypt/Cryption.hpp"
#include <sys/mman.h>
#include <atomic>
#include <sys/fcntl.h>
#include <semaphore.h>
using namespace std;

ProcessManagement::ProcessManagement(){
    sem_t* itemSemaphore = sem_open("items_semaphore", O_CREAT, 0666, 0);
    sem_t* emptySlotsSemaphore = sem_open("empty_slots_semaphore", O_CREAT, 0666, 1000);

    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shmFd, sizeof(SharedMemory));
    sharedMem = static_cast<SharedMemory *>(mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0));
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}

bool ProcessManagement::submitToQueue(unique_ptr<Task>task){
    sem_wait(emptySlotsSemaphore);
    unique_lock<mutex> lock(queueLock);
    if(sharedMem->size.load() >= 1000){
        return false;
    }
    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());
    sharedMem->rear = (sharedMem->rear + 1)%1000;
    sharedMem->size.fetch_add(1);

    lock.unlock();
    sem_post(itemSemaphore);

    int pid = fork();
    if(pid<0){
        return false;
    }
    else if(pid>0){
        cout<<"Entering the parent process"<<endl;
    }
    else{
        cout<<"Entering the child process"<<endl;
        executeTasks();
        cout<<"Exiting the child processs"<<endl;
        exit(0);
    }
    return true;

}
void ProcessManagement::executeTasks(){
    sem_wait(itemSemaphore);
    unique_lock<mutex> lock(queueLock);
    char taskStr[256];
    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front + 1)%1000;
    sharedMem->size.fetch_sub(1);

    lock.unlock();
    sem_post(emptySlotsSemaphore);

    cout<<"Executing child process"<<endl;
    executeCryption(taskStr);   
}

ProcessManagement::~ProcessManagement(){
    munmap(sharedMem, sizeof(SharedMemory));
    shm_unlink(SHM_NAME);
}
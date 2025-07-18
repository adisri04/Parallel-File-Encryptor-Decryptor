#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include "Task.hpp"
#include <queue>
#include <memory>
#include <semaphore.h>
using namespace std;

class ProcessManagement{
        sem_t* itemSemaphore;
        sem_t* emptySlotsSemaphore;
    public:
        ProcessManagement();
        ~ProcessManagement();
        bool submitToQueue(unique_ptr<Task> task);
        void executeTasks();
    private:
        struct SharedMemory {
            atomic<int>size;
            char tasks[1000][256];
            int front;
            int rear;
            void printSharedMemory() {
                cout<<size<<endl;
            }
        };
        SharedMemory* sharedMem;
        int shmFd;
        const char* SHM_NAME = "/my_queue";
        mutex queueLock;
};

#endif
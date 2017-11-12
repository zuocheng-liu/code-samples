#ifndef ASYNCRONOUS_TASK_TASK_MANAGER_H
#define ASYNCRONOUS_TASK_TASK_MANAGER_H

#include <queue>

#include "Task.h"

class TaskManager {
    public :
        void AddTask() {
        }
    private :
        std::queue task_queue_;
};

#endif

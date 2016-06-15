#ifndef ASYNC_TASK_H_
#define ASYNC_TASK_H_

#include <queue>

class AsyncTask {
    public :
        virtual void execute();
};

class AsyncTaskManager {
    public :
        inline void addTask(AsyncTask& task) {
            _taskQueue.push(&task);
        }
        inline void execute() {
            while (!_taskQueue.empty) {
                const AsyncTask *task = _taskQueue.front();
                task->execute();
            }
        }
    private :
        std::queue<AsyncTask *> _taskQueue;
        
};

#endif

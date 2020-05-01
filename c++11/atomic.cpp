#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
using namespace std;

std::mutex id_mutex;
std::condition_variable condition;

int64_t _exp_id;
std::atomic<int64_t> _curr_unique_id;

int64_t generateUniqueId() { 
    ++_exp_id;
    return ++_curr_unique_id;
}

int main() {
    _exp_id = 0;
    _curr_unique_id = 0;
    std::vector< std::thread > workers;
    size_t thread_num = 100;
    for(size_t i = 0;i < thread_num ;++i) {
        workers.emplace_back([]{
            for (int j = 0; j < 10000; ++j) {
                generateUniqueId();
            };
            return true;
        });
    }
    for(std::thread &worker: workers) {
        worker.join();
    }
    cout << "_curr_unique_id:" << _curr_unique_id << endl;
    cout << "_exp_id:" << _curr_unique_id << endl;
    return 0;
}

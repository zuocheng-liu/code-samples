#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
using namespace std;

int64_t _exp_id;
std::atomic<int64_t> _curr_unique_id;
volatile bool run = false;

int64_t generateUniqueId() { 
    ++_exp_id;
    return ++_curr_unique_id;
}

int main() {
    _exp_id = 0;
    _curr_unique_id = 0;
    run = false;
    std::vector< std::thread > workers;
    size_t thread_num = 100;
    for(size_t i = 0;i < thread_num ;++i) {
        workers.emplace_back([]{
            for (;;) {
              if (!run) continue;
              for (int j = 0; j < 1e7; ++j) {
                  generateUniqueId();
              };
            return true;
            }
        });
    }
    run = true;
    for(std::thread &worker: workers) {
        worker.join();
    }
    cout << "_curr_unique_id:" << _curr_unique_id << endl;
    cout << "_exp_id:" << _exp_id << endl;
    return 0;
}

//vim: set ts=4 sw=4 sts=4 tw=100 expandtab smarttab:

#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__
#include <pthread.h>
#include <iostream>

template<class T>
class CSingleton {
 public:
   static T& GetInstance() {
     if (NULL == instance_) {
       instance_ = new T();
     }
     return *instance_;
   }

   static T& GetThreadSpecificInstance() {
       pthread_once(&g_once_control, InitOnce);
       T *thread_specific_instance = (T*)pthread_getspecific(g_thread_data_key);
       if (NULL == thread_specific_instance) {
           thread_specific_instance = new T();
           pthread_setspecific(g_thread_data_key, (void*)thread_specific_instance);
       }
       return *thread_specific_instance;
   }

 private:
   static void InitOnce(void) {
     pthread_key_create(&g_thread_data_key, NULL);
   }

   static T *instance_;
   static pthread_once_t g_once_control;
   static pthread_key_t g_thread_data_key;
};

template<class T>
T* CSingleton<T>::instance_ = NULL;

template<class T>
pthread_once_t CSingleton<T>::g_once_control = PTHREAD_ONCE_INIT;

template<class T>
pthread_key_t  CSingleton<T>::g_thread_data_key;

#endif // _SINGLETON_HPP_

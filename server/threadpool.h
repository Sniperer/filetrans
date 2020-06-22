#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <iostream>
#include <pthread.h>
#include <vector>
#include <map>
#include <utility>
#include <queue>


typedef void* func(void * argv);

enum thread_status{
    THREAD_RUNNABLE=1,
    THREAD_STOPABLE
};

class thread_task{
    public:
        thread_task(threadpool* _tp):tp(_tp){}
        virtual ~thread_task()=default;
        virtual int exec()=0;
    private:
        threadpool* tp;
};

class threadpool{
    public:
        threadpool(int num);
        ~threadpool();
        void add_task(thread_task* task);
    private:
        int threadpool_size;
        
        static void* thread_loop(void* argv);
        //void quit_loop();

        static pthread_mutex_t task_mutex;
        static pthread_mutex_t thread_status_map_mutex;
        static pthread_cond_t cond;
        static std::queue<thread_task* > task_queue;
        static std::map<pthread_t,thread_status> thread_status_map;
//        static std::vector<pthread_t> thread_seq;
        static int set_thread_status(pthread_t tid,thread_status STAT);
};

#endif


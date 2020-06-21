#include "threadpool.h"
#include <pthread.h>
#include <queue>
#include <map>

pthread_mutex_t threadpool::task_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t threadpool::thread_status_map_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t threadpool::cond=PTHREAD_COND_INITIALIZER;
std::queue<thread_task*> threadpool::task_queue;
std::map<pthread_t,thread_status> threadpool::thread_status_map;

threadpool::threadpool(int num):threadpool_size(num){
    for(int i=0;i<threadpool_size;i++){
        pthread_t tid;
        pthread_create(&tid,NULL,thread_loop,NULL);        
    //    thread_seq.pushback(tid);
    }
}

threadpool::~threadpool(){
    for(auto iter=thread_status_map.begin();iter!=thread_status_map.end();++iter){
        set_thread_status(iter->first,THREAD_STOPABLE);
    }
}

void threadpool::add_task(thread_task* task){
    pthread_mutex_lock(&task_mutex);
    task_queue.push(task);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&task_mutex);    
}

void* threadpool::thread_loop(void* argv){
    pthread_t tid=pthread_self();
    set_thread_status(tid,THREAD_RUNNABLE);
    while(1){
        pthread_mutex_lock(&task_mutex);
        while(task_queue.size()==0){
            pthread_cond_wait(&cond,&task_mutex);
        }
        if(thread_status_map[tid]==THREAD_STOPABLE){
            pthread_mutex_unlock(&task_mutex);
            pthread_exit(NULL);
        }
        if(task_queue.size()==0) continue;
        thread_task *exec_task=task_queue.front();
        task_queue.pop();
        pthread_mutex_unlock(&task_mutex);
        exec_task->exec();
    //    delete(exec_task);
    }
    return NULL;            
}

int threadpool::set_thread_status(pthread_t tid,thread_status STAT){
    pthread_mutex_lock(&thread_status_map_mutex);
    thread_status_map[tid]=STAT;
    pthread_mutex_unlock(&thread_status_map_mutex);
    return 0;
}

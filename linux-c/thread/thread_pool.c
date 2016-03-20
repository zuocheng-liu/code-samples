#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <pthread.h>
 
/* 要执行的任务链表 */
typedef struct tpool_work {
    void*               (*routine)(void*);       /* 任务函数 */
    void                *arg;                    /* 传入任务函数的参数 */
    struct tpool_work   *next;                    
}tpool_work_t;

typedef struct tpool {
    int             shutdown;                    /* 线程池是否销毁 */
    int             max_thr_num;                /* 最大线程数 */
    pthread_t       *thr_id;                    /* 线程ID数组 */
    tpool_work_t    *queue_head;                /* 线程链表 */
    pthread_mutex_t queue_lock;                    
    pthread_cond_t  queue_ready;    
}tpool_t;

/*
 * @brief     创建线程池 
 * @param     max_thr_num 最大线程数
 * @return     0: 成功 其他: 失败  
 */
int tpool_create(int max_thr_num);

/*
 * @brief     销毁线程池 
 */
void tpool_destroy();

/*
 * @brief     向线程池中添加任务
 * @param    routine 任务函数指针
 * @param     arg 任务函数参数
 * @return     0: 成功 其他:失败 
 */
int tpool_add_work(void*(*routine)(void*), void *arg);
static tpool_t *tpool = NULL;
/* 工作者线程函数, 从任务链表中取出任务并执行 */
static void* thread_routine(void *arg) {
    tpool_work_t *work;

    while(1) {
        /* 如果线程池没有被销毁且没有任务要执行，则等待 */
        pthread_mutex_lock(&tpool->queue_lock);
        while(!tpool->queue_head && !tpool->shutdown) {
            pthread_cond_wait(&tpool->queue_ready, &tpool->queue_lock);
        }
        if (tpool->shutdown) {
            pthread_mutex_unlock(&tpool->queue_lock);
            pthread_exit(NULL);
        }
        work = tpool->queue_head;
        tpool->queue_head = tpool->queue_head->next;
        pthread_mutex_unlock(&tpool->queue_lock);

        work->routine(work->arg);
        free(work);
    }

    return NULL;   
}

/*
 * 创建线程池 
 */
int tpool_create(int max_thr_num)
{
    int i;

    tpool = calloc(1, sizeof(tpool_t));
    if (!tpool) {
        printf("%s: calloc failed\n", __FUNCTION__);
        exit(1);
    }

    /* 初始化 */
    tpool->max_thr_num = max_thr_num;
    tpool->shutdown = 0;
    tpool->queue_head = NULL;
    if (pthread_mutex_init(&tpool->queue_lock, NULL) !=0) {
        printf("%s: pthread_mutex_init failed, errno:%d, error:%s\n",
                58:             __FUNCTION__, errno, strerror(errno));
        exit(1);
    }
    if (pthread_cond_init(&tpool->queue_ready, NULL) !=0 ) {
        printf("%s: pthread_cond_init failed, errno:%d, error:%s\n", 
                63:             __FUNCTION__, errno, strerror(errno));
        exit(1);
    }

    /* 创建工作者线程 */
    tpool->thr_id = calloc(max_thr_num, sizeof(pthread_t));
    if (!tpool->thr_id) {
        printf("%s: calloc failed\n", __FUNCTION__);
        exit(1);
    }
    for (i = 0; i < max_thr_num; ++i) {
        if (pthread_create(&tpool->thr_id[i], NULL, thread_routine, NULL) != 0){
            printf("%s:pthread_create failed, errno:%d, error:%s\n", __FUNCTION__, 
                    76:                 errno, strerror(errno));
            exit(1);
        }

    }    

    return 0;
}

/* 销毁线程池 */
    void
tpool_destroy()
{
    int i;
    tpool_work_t *member;

    if (tpool->shutdown) {
        return;
    }
    tpool->shutdown = 1;

    /* 通知所有正在等待的线程 */
    pthread_mutex_lock(&tpool->queue_lock);
    pthread_cond_broadcast(&tpool->queue_ready);
    pthread_mutex_unlock(&tpool->queue_lock);
    for (i = 0; i < tpool->max_thr_num; ++i) {
        pthread_join(tpool->thr_id[i], NULL);
    }
    free(tpool->thr_id);

    while(tpool->queue_head) {
        member = tpool->queue_head;
        tpool->queue_head = tpool->queue_head->next;
        free(member);
    }

    pthread_mutex_destroy(&tpool->queue_lock);    
    pthread_cond_destroy(&tpool->queue_ready);

    free(tpool);    
}

/* 向线程池添加任务 */
    int
tpool_add_work(void*(*routine)(void*), void *arg)
{
    tpool_work_t *work, *member;

    if (!routine){
        printf("%s:Invalid argument\n", __FUNCTION__);
        return -1;
    }

    work = malloc(sizeof(tpool_work_t));
    if (!work) {
        printf("%s:malloc failed\n", __FUNCTION__);
        return -1;
    }
    work->routine = routine;
    work->arg = arg;
    work->next = NULL;

    pthread_mutex_lock(&tpool->queue_lock);    
    member = tpool->queue_head;
    if (!member) {
        tpool->queue_head = work;
    } else {
        while(member->next) {
            member = member->next;
        }
        member->next = work;
    }
    /* 通知工作者线程，有新任务添加 */
    pthread_cond_signal(&tpool->queue_ready);
    pthread_mutex_unlock(&tpool->queue_lock);

    return 0;    
}



#pragma once
#ifndef threadpool_H_
#define threadpool_H_

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>


struct job {
	void* (*callback_func)(void *arg);
	void *arg;
	struct job *next;
};

struct threadpool {
	int thread_num;
	int queue_max_num;
	int queue_cur_num;
	int queue_close;
	int pool_close;
	struct job *head;
	struct job *rear;
	pthread_t *pthreads;
	pthread_mutex_t mutex;
	pthread_cond_t queue_empty;
	pthread_cond_t queue_not_empty;
	pthread_cond_t queue_not_full;
};

void* threadpool_func(void *arg);

struct threadpool* threadpool_init(int thread_num, int queue_max_num);

int threadpool_add_job(struct threadpool *pool, void* (*callback_func)(void *arg), void *arg);

int threadpool_destroy(struct threadpool *pool);

#endif
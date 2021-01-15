#include "threadpool.h"

#include"threadpool.h"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

void* threadpool_func(void* arg) {
	struct threadpool *pool = (struct threadpool*)arg;
	struct job *pjob = NULL;
	while (1) {
		pthread_mutex_lock(&(pool->mutex));
		while ((pool->queue_cur_num == 0) && !pool->pool_close) {
			pthread_cond_wait(&(pool->queue_not_empty), &(pool->mutex));
		}
		if (pool->pool_close) {
			pthread_mutex_unlock(&(pool->mutex));
			pthread_exit(NULL);
		}
		pool->queue_cur_num--;
		pjob = pool->head;
		if (pool->queue_cur_num == 0) {
			pool->head = pool->rear = NULL;
			pthread_cond_signal(&(pool->queue_empty));
		}
		else {
			pool->head = pjob->next;
		}
		if (pool->queue_cur_num == pool->queue_max_num - 1) {
			pthread_cond_broadcast(&(pool->queue_not_full));
		}
		pthread_mutex_unlock(&(pool->mutex));
		(*(pjob->callback_func))(pjob->arg);
		free(pjob);
		pjob = NULL;
	}
}

struct threadpool* threadpool_init(int thread_num, int queue_max_num) {
	struct threadpool *pool = NULL;
	pool =(struct threadpool *) malloc(sizeof(struct threadpool));
	if (pool == NULL) {
		perror("malloc threadpool failed");
		return NULL;
	}
	pool->thread_num = thread_num;
	pool->queue_max_num = queue_max_num;
	pool->queue_cur_num = 0;
	pool->head = NULL;
	pool->rear = NULL;
	if (pthread_mutex_init(&(pool->mutex), NULL)) {
		perror("init mutex failed");
		return NULL;
	}
	if (pthread_cond_init(&(pool->queue_empty), NULL)) {
		perror("init queue_empty failed");
		return NULL;
	}
	if (pthread_cond_init(&(pool->queue_not_empty), NULL)) {
		perror("init queue_not_empty failed");
		return NULL;
	}
	if (pthread_cond_init(&(pool->queue_not_full), NULL)) {
		perror("init queue_not_full failed");
		return NULL;
	}
	pool->pthreads = (pthread_t *)malloc(sizeof(pthread_t)*thread_num);
	if (pool->pthreads == NULL) {
		perror("malloc pthreads failed");
		return NULL;
	}
	pool->queue_close = 0;
	pool->pool_close = 0;
	int i;
	for (i = 0; i < pool->thread_num; ++i) {
		pthread_create(&(pool->pthreads[i]), NULL, threadpool_func, (void *)pool);
	}
	return pool;
}

int threadpool_add_job(struct threadpool* pool, void* (*callback_func)(void *arg), void *arg) {
	assert(pool != NULL);
	assert(callback_func != NULL);
	assert(arg != NULL);
	pthread_mutex_lock(&(pool->mutex));
	while ((pool->queue_cur_num == pool->queue_max_num) && !(pool->queue_close || pool->pool_close)) {
		pthread_cond_wait(&(pool->queue_not_full), &(pool->mutex));
	}
	if (pool->queue_close || pool->pool_close) {
		pthread_mutex_unlock(&(pool->mutex));
		return -1;
	}
	struct job *pjob = (struct job*)malloc(sizeof(struct job));
	if (pjob == NULL) {
		pthread_mutex_unlock(&(pool->mutex));
		return -1;
	}
	pjob->callback_func = callback_func;
	pjob->arg = arg;
	pjob->next = NULL;
	if (pool->head == NULL) {
		pool->head = pool->rear = pjob;
		pthread_cond_broadcast(&(pool->queue_not_empty));
	}
	else {
		pool->rear->next = pjob;
		pool->rear = pjob;
	}
	pool->queue_cur_num++;
	pthread_mutex_unlock(&(pool->mutex));
	return 0;
}

int threadpool_destroy(struct threadpool *pool) {
	assert(pool != NULL);
	pthread_mutex_lock(&(pool->mutex));
	if (pool->queue_close || pool->pool_close) {
		pthread_mutex_unlock(&(pool->mutex));
		return -1;
	}
	pool->queue_close = 1;
	while (pool->queue_cur_num != 0) {
		pthread_cond_wait(&(pool->queue_empty), &(pool->mutex));
	}
	pool->pool_close = 1;
	pthread_mutex_unlock(&(pool->mutex));
	pthread_cond_broadcast(&(pool->queue_not_empty));
	pthread_cond_broadcast(&(pool->queue_not_full));
	int i;
	for (i = 0; i < pool->thread_num; ++i) {
		pthread_join(pool->pthreads[i], NULL);
	}
	pthread_mutex_destroy(&(pool->mutex));
	pthread_cond_destroy(&(pool->queue_empty));
	pthread_cond_destroy(&(pool->queue_not_empty));
	pthread_cond_destroy(&(pool->queue_not_full));
	free(pool->pthreads);
	struct job *p;
	while (pool->rear != NULL) {
		p = pool->head;
		pool->head = p->next;
		free(p);
	}
	free(pool);
	return 0;
}


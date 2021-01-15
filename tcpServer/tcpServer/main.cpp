#include <iostream>
#include "TcpServer.h"

int CALL_METHOD  myRecv(RECV_DATA_STRUCT& recvData,void* userData)
{
	echo2File("myRecv enter,fd:"+std::to_string(recvData.fd)+"dataLen:"+std::to_string(recvData.data.size()));
	NETWORK_SDK* httpSdk = static_cast<NETWORK_SDK*>(userData);
	std::string retStatusCodeStr;
		httpSdk->SetHttpStatusCode(200, retStatusCodeStr);
		httpSdk->SendDataToClient(recvData.fd, retStatusCodeStr);
	return 0;
}

class RecvClient
{
	public:
	int myRecv(RECV_DATA_STRUCT& recvData,void* userData)
	{
	return 0;
	}
};
//
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//#include<errno.h>
//#include<fcntl.h>
//#include<sys/types.h>
//#include<netinet/in.h>
//#include<sys/epoll.h>
//#include<sys/socket.h>
//#include<arpa/inet.h>
//#include"threadpool.h"
//#include<dlfcn.h>
//#include<time.h>
//#define MAX_LEN 1024
//#define MAX_SIZE 100
//
//typedef struct MyWorkStruct{
//	int epollFd;
//	int sockfd;
//}MyStructWork;
//
//
//void *worker(void *arg) {
//	char buf[BUFFER_LENGTH] = {0};
//	MyWorkStruct wSA = *(MyWorkStruct*)arg;
//	int sock = wSA.sockfd;//*(int *)arg;
//	int fd = wSA.epollFd;
//	free(arg);
//	int len = -1;
//	int count = 0;
//	int reuse = 1, totalSize=0;
//	//fcntl(sock, F_SETFL, O_NONBLOCK);
//	//setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
//	while ((len = recv(sock, buf, 1, 0))>0)
//	{
//		/*memset(buf,0,sizeof(buf));
//		count += 1;
//		printf("worker,len=%d",len);*/
//		printf("nready:%d", len);
//		RECV_DATA_STRUCT recvData;
//		recvData.fd = fd;
//		recvData.data = buf;
//		//m_recvFromClientCallBackPtr(recvData, m_userData);
//		totalSize += len;
//		echo2File("fd:" + std::to_string(fd) + "nreadyLenth:" + std::to_string(len) + buf + "totalSize:" + std::to_string(totalSize));
//		{
//			//printf("Recv:%s,%d bytes(s),clientfd:%d\n", buffer, len, clientfd);
//			//memset(buf, 0, sizeof(buf));
//			/*std::string statusCodeStr;
//
//			close(clientfd);
//			struct epoll_event ev;
//			ev.events = EPOLLIN;
//			ev.data.fd = clientfd;
//			epoll_ctl(m_pSR.epfd, EPOLL_CTL_DEL, clientfd, &ev);*/
//			//send(clientfd, statusCodeStr.c_str(), strlen(buffer), 0);
//		}
//	}
//	
//
//	memset(buf, 0, sizeof(buf));
//	NETWORK_SDK httpSdk;
//	std::string retStatusCodeStr;
//	httpSdk.SetHttpStatusCode(200, retStatusCodeStr);
//	sprintf(buf, retStatusCodeStr.c_str());
//	 reuse = 1;
//	fcntl(sock, F_SETFL, O_NONBLOCK);
//	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
//	send(sock, retStatusCodeStr.c_str(), retStatusCodeStr.size(), 0);
//	//close(sock);
//	if (len < 0)
//	{
//		close(sock);
//		struct epoll_event ev;
//		ev.events = EPOLLIN;
//		ev.data.fd = sock;
//		epoll_ctl(fd, EPOLL_CTL_DEL, sock, &ev);
//	}
//	else if (len == 0) //disconnect
//	{
//		close(sock);
//		struct epoll_event ev;
//		ev.events = EPOLLIN;
//		ev.data.fd = sock;
//		epoll_ctl(fd, EPOLL_CTL_DEL, sock, &ev);
//	}
//	printf("worker end,len=%d,count=%d", len,count);
//}
//
//void *worker2(void *arg) {
//	char buf[MAX_LEN] = { 0 };
//	MyWorkStruct wSA = *(MyWorkStruct*)arg;
//	int fd = wSA.sockfd;//*(int *)arg;
//	int epoll_fd = wSA.epollFd;
//	free(arg);
//	int clientfd = fd;
//	char buffer[BUFFER_LENGTH] = { 0 };
//	int len = 0, totalSize = 0;
//	int reuse = 1;
//	fcntl(clientfd, F_SETFL, O_NONBLOCK);
//	setsockopt(clientfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
//	while (len = recv(clientfd, buffer, BUFFER_LENGTH, 0) > 0)
//	{
//		printf("nready:%d", len);
//		RECV_DATA_STRUCT recvData;
//		recvData.fd = clientfd;
//		recvData.data = buffer;
//		//m_recvFromClientCallBackPtr(recvData, m_userData);
//		totalSize += len;
//		echo2File("fd:" + std::to_string(clientfd) + "nreadyLenth:" + std::to_string(len) + buffer + "totalSize:" + std::to_string(totalSize));
//		{
//			//printf("Recv:%s,%d bytes(s),clientfd:%d\n", buffer, len, clientfd);
//			memset(buffer, 0, sizeof(buffer));
//			/*std::string statusCodeStr;
//
//			close(clientfd);
//			struct epoll_event ev;
//			ev.events = EPOLLIN;
//			ev.data.fd = clientfd;
//			epoll_ctl(m_pSR.epfd, EPOLL_CTL_DEL, clientfd, &ev);*/
//			//send(clientfd, statusCodeStr.c_str(), strlen(buffer), 0);
//		}
//	}
//	printf("recvAfter:%d\n", len);
//	std::string statusCodeStr;
//	NETWORK_SDK httpSDK;
//	httpSDK.SetHttpStatusCode(200, statusCodeStr);
//	//SetHttpHeaders();
//	//statusCodeStr += "\r\n\r\n";
//	fcntl(clientfd, F_SETFL, O_NONBLOCK);
//	setsockopt(clientfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
//	send(clientfd, statusCodeStr.c_str(), statusCodeStr.size(), 0);
//	if (len < 0)
//	{
//		close(clientfd);
//		struct epoll_event ev;
//		ev.events = EPOLLIN;
//		ev.data.fd = clientfd;
//		epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientfd, &ev);
//	}
//	else if (len == 0) //disconnect
//	{
//		close(clientfd);
//		struct epoll_event ev;
//		ev.events = EPOLLIN;
//		ev.data.fd = clientfd;
//		epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientfd, &ev);
//	}
//
//echo2File("clientWork finish");
//	//printf("worker end,len=%d,count=%d", len, count);
//}
//
//int main()
//{
//	struct sigaction sa;
//	sa.sa_handler = SIG_IGN;
//	sigaction(SIGPIPE, &sa, 0);
//	/*NETWORK_SDK m_httpSDK;
//	RecvClient m_rec;
//	m_httpSDK.SetRecvRegisterCB(myRecv,&m_httpSDK);
//	m_httpSDK.HttpListen("0.0.0.0",6665);*/
//	int server_sockfd;
//	int client_sockfd;
//	struct sockaddr_in server_addr;
//	struct sockaddr_in client_addr;
//	int sin_size;
//	int epoll_fd;
//	struct epoll_event ev;
//	struct epoll_event events[MAX_SIZE] = {0};
//	int nfds;
//	struct threadpool *pool;
//	pool = threadpool_init(16, 100);
//	memset(&server_addr, 0, sizeof(server_addr));
//	server_addr.sin_family = AF_INET;
//	server_addr.sin_addr.s_addr = INADDR_ANY;
//	server_addr.sin_port = htons(6665);
//	if ((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
//		perror("socket");
//		exit(EXIT_FAILURE);
//	}
//	int on = 1;
//	fcntl(client_sockfd, F_SETFL, O_NONBLOCK);
//	setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
//	if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0) {
//		perror("bind");
//		exit(EXIT_FAILURE);
//	}
//	listen(server_sockfd, 5);
//	int old_option = fcntl(server_sockfd, F_GETFL);
//	int new_option = old_option | O_NONBLOCK;
//	fcntl(server_sockfd, F_SETFL, new_option);
//
//	if ((epoll_fd = epoll_create(1)) < 0) {
//		perror("create");
//		exit(EXIT_FAILURE);
//	}
//	ev.events = EPOLLIN;
//	ev.data.fd = server_sockfd;
//	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_sockfd, &ev) < 0) {
//		perror("ctl");
//		exit(EXIT_FAILURE);
//	}
//	while (1) {
//		nfds = epoll_wait(epoll_fd, events, MAX_SIZE, 5);
//		if (nfds < 0) {
//			perror("wait");
//			exit(EXIT_FAILURE);
//		}
//		int i;
//		for (i = 0; i < nfds; ++i) {
//			//	if (events[i].data.fd == server_sockfd) {
//			//		sin_size = sizeof(struct sockaddr_in);
//			//		if ((client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, (socklen_t*)&sin_size)) < 0) {
//			//			perror("accept");
//			//			exit(EXIT_FAILURE);
//			//		}
//			//		if (client_sockfd != -1)
//			//		{
//			//			fcntl(client_sockfd, F_SETFL, fcntl(client_sockfd, F_GETFD, 0) | O_NONBLOCK);
//			//			fcntl(client_sockfd, F_SETFL, O_NONBLOCK);
//			//			ev.events = EPOLLIN | EPOLLET;
//			//			ev.data.fd = client_sockfd;
//			//			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sockfd, &ev) < 0) {
//			//				perror("CTL");
//			//				exit(EXIT_FAILURE);
//			//			}
//			//			printf("accpet!\n");
//			//		}
//			//	}
//			//	else {
//			//		printf("pool in ");
//			//		int *ptr = (int*)malloc(sizeof(int));
//			//		*ptr = events[i].data.fd;
//			//		int reuse = 1;
//			//		fcntl(*ptr, F_SETFL, O_NONBLOCK);
//			//		setsockopt(*ptr, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
//			//		//static ThreadPool m_threadPool(10);
//			//	//	m_threadPool.enqueue(
//			//	//		/*[fd,&events,m_pSR]() {
//			//	//		clientWork(fd, events, *m_pSR);
//			//	//		});*/
//			//	//		worker, (void *)ptr, epoll_fd);
//			//	//	//clientWork(fd,events,*m_pSR);
//			//	//}
//					MyWorkStruct* myWorkStruct = (MyWorkStruct*)malloc(sizeof(MyWorkStruct));
//					//MyWorkStruct* myWorkStruct = new MyWorkStruct;
//					myWorkStruct->epollFd = epoll_fd;
//					myWorkStruct->sockfd = events[i].data.fd;
//					//int *ptr = (int*)malloc(sizeof(int));
//					//*ptr = events[i].data.fd;
//					//threadpool_add_job(pool, worker, (void *)myWorkStruct);
//					static ThreadPool m_threadPool(10);
//							m_threadPool.enqueue(
//								/*[fd,&events,m_pSR]() {
//								clientWork(fd, events, *m_pSR);
//								});*/
//								worker, (void *)myWorkStruct);
//							//clientWork(fd,events,*m_pSR);
//						
//					/*struct epoll_event ev;
//					ev.events = EPOLLIN;
//					ev.data.fd = events[i].data.fd;
//					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &ev);*/
//					//worker((void*)ptr);
//					}
//					
//				}
//			//}
//		threadpool_destroy(pool);
//		close(server_sockfd);
//	
//
//	//	
//			//int fd = events[i].data.fd;
//			//int iRet = 0;
//			//int sockfd = server_sockfd;
//			////std::lock_guard<std::mutex> m_lgmutex(g_mutex);
//			////int sockfd = isListenFds(fd, m_pSR.socketfds);
//			//printf("sockfd:%d", sockfd);
//			///*	if (fdIsRun.find(sockfd) != fdIsRun.end() && fdIsRun[sockfd])
//			//	{
//			//		return iRet;
//			//	}
//			//	fdIsRun[sockfd] = true;*/
//			//echo2File("sockfd:" + std::to_string(sockfd) + "-fd:" + std::to_string(fd));
//			//if (fd == sockfd) //listen
//			//{
//			//	printf("listen:%d", sockfd);
//			//	struct sockaddr_in client_addr;
//			//	memset(&client_addr, 0, sizeof(struct sockaddr_in));
//			//	int reuse = 1;
//			//	socklen_t client_len = sizeof(client_addr);
//			//	fcntl(sockfd, F_SETFL, O_NONBLOCK);
//			//	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
//			//	int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
//			//	if (clientfd != -1)
//			//	{
//			//		fcntl(clientfd, F_SETFL, O_NONBLOCK);
//
//			//		setsockopt(clientfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
//			//		struct epoll_event ev;
//			//		ev.events = EPOLLIN | EPOLLET;
//			//		ev.data.fd = clientfd;
//			//		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientfd, &ev);
//			//		echo2File("accept finish");
//			//	}
//			//}
//			//else
//			//{
//			//	MyWorkStruct* myWorkStruct = (MyWorkStruct*)malloc(sizeof(MyWorkStruct));
//			//	//		//MyWorkStruct* myWorkStruct = new MyWorkStruct;
//			//			myWorkStruct->epollFd = epoll_fd;
//			//			myWorkStruct->sockfd = events[i].data.fd;
//			//	//int *ptr = (int*)malloc(sizeof(int));
//			//	//*ptr = events[i].data.fd;
//			//	worker((void*)myWorkStruct);
//			//		//	worker2((void*)myWorkStruct);
//			//	//fdIsRun[sockfd] = false;
//			//}
//		/*}
//	}*/
//	return 0;
//
//
//	
//}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/poll.h>

#include <errno.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define SERVER_PORT		8080
#define SERVER_IP		"127.0.0.1"
#define MAX_BUFFER		128
#define MAX_EPOLLSIZE	100000
#define MAX_THREAD		80
#define MAX_PORT		100

#define CPU_CORES_SIZE	8

#define TIME_SUB_MS(tv1, tv2)  ((tv1.tv_sec - tv2.tv_sec) * 1000 + (tv1.tv_usec - tv2.tv_usec) / 1000)

static int ntySetNonblock(int fd) {
	int flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0) return flags;
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0) return -1;
	return 0;
}

static int ntySetReUseAddr(int fd) {
	int reuse = 1;
	return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
}

static int ntySetAlive(int fd) {
	int alive = 1;
	int idle = 60;
	int interval = 5;
	int count = 2;

	setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void*)&alive, sizeof(alive));
	setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void*)&idle, sizeof(idle));
	setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void*)&interval, sizeof(interval));
	setsockopt(fd, SOL_TCP, TCP_KEEPCNT, (void*)&count, sizeof(count));
}


/** **** ******** **************** thread pool **************** ******** **** **/

#define LL_ADD(item, list) { \
	item->prev = NULL; \
	item->next = list; \
	list = item; \
}

#define LL_REMOVE(item, list) { \
	if (item->prev != NULL) item->prev->next = item->next; \
	if (item->next != NULL) item->next->prev = item->prev; \
	if (list == item) list = item->next; \
	item->prev = item->next = NULL; \
}

typedef struct worker {
	pthread_t thread;
	int terminate;
	struct workqueue *workqueue;
	struct worker *prev;
	struct worker *next;
} worker_t;

typedef struct job {
	void(*job_function)(struct job *job);
	void *user_data;
	struct job *prev;
	struct job *next;
} job_t;

typedef struct workqueue {
	struct worker *workers;
	struct job *waiting_jobs;
	pthread_mutex_t jobs_mutex;
	pthread_cond_t jobs_cond;
} workqueue_t;


static void *worker_function(void *ptr) {
	worker_t *worker = (worker_t *)ptr;
	job_t *job;

	while (1) {
		pthread_mutex_lock(&worker->workqueue->jobs_mutex);
		while (worker->workqueue->waiting_jobs == NULL) {
			if (worker->terminate) break;
			pthread_cond_wait(&worker->workqueue->jobs_cond, &worker->workqueue->jobs_mutex);
		}
		if (worker->terminate) break;
		job = worker->workqueue->waiting_jobs;
		if (job != NULL) {
			LL_REMOVE(job, worker->workqueue->waiting_jobs);
		}
		pthread_mutex_unlock(&worker->workqueue->jobs_mutex);

		if (job == NULL) continue;

		/* Execute the job. */
		job->job_function(job);
	}

	free(worker);
	pthread_exit(NULL);
}

int workqueue_init(workqueue_t *workqueue, int numWorkers) {
	int i;
	worker_t *worker;
	pthread_cond_t blank_cond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t blank_mutex = PTHREAD_MUTEX_INITIALIZER;

	if (numWorkers < 1) numWorkers = 1;

	memset(workqueue, 0, sizeof(*workqueue));
	memcpy(&workqueue->jobs_mutex, &blank_mutex, sizeof(workqueue->jobs_mutex));
	memcpy(&workqueue->jobs_cond, &blank_cond, sizeof(workqueue->jobs_cond));

	for (i = 0; i < numWorkers; i++) {
		if ((worker =(worker_t*) malloc(sizeof(worker_t))) == NULL) {
			perror("Failed to allocate all workers");
			return 1;
		}

		memset(worker, 0, sizeof(*worker));
		worker->workqueue = workqueue;

		if (pthread_create(&worker->thread, NULL, worker_function, (void *)worker)) {
			perror("Failed to start all worker threads");
			free(worker);
			return 1;
		}

		LL_ADD(worker, worker->workqueue->workers);
	}

	return 0;
}


void workqueue_shutdown(workqueue_t *workqueue) {

	worker_t *worker = NULL;
	for (worker = workqueue->workers; worker != NULL; worker = worker->next) {
		worker->terminate = 1;
	}


	pthread_mutex_lock(&workqueue->jobs_mutex);
	workqueue->workers = NULL;
	workqueue->waiting_jobs = NULL;
	pthread_cond_broadcast(&workqueue->jobs_cond);
	pthread_mutex_unlock(&workqueue->jobs_mutex);

}


void workqueue_add_job(workqueue_t *workqueue, job_t *job) {

	pthread_mutex_lock(&workqueue->jobs_mutex);

	LL_ADD(job, workqueue->waiting_jobs);

	pthread_cond_signal(&workqueue->jobs_cond);
	pthread_mutex_unlock(&workqueue->jobs_mutex);

}

static workqueue_t workqueue;
void threadpool_init(void) {
	workqueue_init(&workqueue, MAX_THREAD);
}


/** **** ******** **************** thread pool **************** ******** **** **/

typedef struct client {
	int fd;
	char rBuffer[MAX_BUFFER];
	int length;
} client_t;

void *client_cb(void *arg) {
	int clientfd = *(int *)arg;
	char buffer[MAX_BUFFER] = { 0 };

	int childpid = getpid();

	while (1) {
		bzero(buffer, MAX_BUFFER);
		ssize_t length = recv(clientfd, buffer, MAX_BUFFER, 0); //bio
		if (length > 0) {
			//printf(" PID:%d --> buffer: %s\n", childpid, buffer);

			int sLen = send(clientfd, buffer, length, 0);
			//printf(" PID:%d --> sLen: %d\n", childpid, sLen);
		}
		else if (length == 0) {
			printf(" PID:%d client disconnect\n", childpid);
			break;
		}
		else {
			printf(" PID:%d errno:%d\n", childpid, errno);
			break;
		}
	}
}


static int nRecv(int sockfd, void *data, size_t length, int *count) {
	int left_bytes;
	int read_bytes;
	int res;
	int ret_code;

	unsigned char *p;

	struct pollfd pollfds;
	pollfds.fd = sockfd;
	pollfds.events = (POLLIN | POLLERR | POLLHUP);

	read_bytes = 0;
	ret_code = 0;
	p = (unsigned char *)data;
	left_bytes = length;

	while (left_bytes > 0) {

		read_bytes = recv(sockfd, p, left_bytes, 0);
		if (read_bytes > 0) {
			printf("read:%d",read_bytes);
			left_bytes = read_bytes;
			//p += read_bytes;
			continue;
		}
		else if (read_bytes < 0) {
			if (!(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)) {
				ret_code = (errno != 0 ? errno : EINTR);
			}
		}
		else {
			ret_code = ENOTCONN;
			break;
		}

		res = poll(&pollfds, 1, 5);
		if (pollfds.revents & POLLHUP) {
			ret_code = ENOTCONN;
			break;
		}
		if (res < 0) {
			if (errno == EINTR) {
				continue;
			}
			ret_code = (errno != 0 ? errno : EINTR);
		}
		else if (res == 0) {
			ret_code = ETIMEDOUT;
			break;
		}
		sleep(0.05);

	}

	if (count != NULL) {
		*count = length - left_bytes;
	}
	//printf("nRecv:%s, ret_code:%d, count:%d\n", (char*)data, ret_code, *count);
	if (read_bytes > 0) {
		printf("read:%d", read_bytes);
		left_bytes = read_bytes;
		//p += read_bytes;
		//continue;
	}
	else if (read_bytes < 0) {
		if (!(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)) {
			ret_code = (errno != 0 ? errno : EINTR);
		}
	}
	else {
		ret_code = ENOTCONN;
		//break;
	}

	res = poll(&pollfds, 1, 5);
	if (pollfds.revents & POLLHUP) {
		ret_code = ENOTCONN;
		//break;
	}
	if (res < 0) {
		if (errno == EINTR) {
			//continue;
		}
		ret_code = (errno != 0 ? errno : EINTR);
	}
	else if (res == 0) {
		ret_code = ETIMEDOUT;
		//break;
	}

	printf("end:%d,ret_code=%d", read_bytes, ret_code);
	sleep(0.05);
	return ret_code;
}

void epoll_et_loop(int sockfd)
{
	char buffer[MAX_BUFFER];
	int ret;

	while (1)
	{
		memset(buffer, 0, MAX_BUFFER);
		ret = recv(sockfd, buffer, MAX_BUFFER, 0);
		if (ret == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				printf(" read all data\n");
				break;
			}
			close(sockfd);
			break;
		}
		else if (ret == 0)
		{
			printf(" disconnect\n");
			close(sockfd);
			break;
		}
		else
			printf("Recv:%s, %d Bytes\n", buffer, ret);
	}

}

static int nSend(int sockfd, const void *buffer, int length, int flags) {

	int wrotelen = 0;
	int writeret = 0;

	unsigned char *p = (unsigned char *)buffer;

	struct pollfd pollfds = { 0 };
	pollfds.fd = sockfd;
	pollfds.events = (POLLOUT | POLLERR | POLLHUP);

	do {
		int result = poll(&pollfds, 1, 5);
		if (pollfds.revents & POLLHUP) {

			printf(" ntySend errno:%d, revent:%x\n", errno, pollfds.revents);
			return -1;
		}

		if (result < 0) {
			if (errno == EINTR) continue;

			printf(" ntySend errno:%d, result:%d\n", errno, result);
			return -1;
		}
		else if (result == 0) {

			printf(" ntySend errno:%d, socket timeout \n", errno);
			return -1;
		}

		writeret = send(sockfd, p + wrotelen, length - wrotelen, flags);
		if (writeret <= 0)
		{
			break;
		}
		wrotelen += writeret;

	} while (wrotelen < length);

	return wrotelen;
}


static int curfds = 1;
static int nRun = 0;
int sockfds[MAX_PORT] = { 0 };


void client_job(job_t *job) {


	client_t *rClient = (client_t*)job->user_data;
	int clientfd = rClient->fd;

	char buffer[MAX_BUFFER];
	bzero(buffer, MAX_BUFFER);

	int length = 0;
	int ret = nRecv(clientfd, buffer, MAX_BUFFER, &length);
	if (length > 0) {
		/*if (nRun || buffer[0] == 'a') {
			printf(" TcpRecv --> curfds : %d, buffer: %s\n", curfds, buffer);

			nSend(clientfd, buffer, strlen(buffer), 0);
		}*/
		NETWORK_SDK httpSdk ;
			std::string retStatusCodeStr;
				httpSdk.SetHttpStatusCode(200, retStatusCodeStr);
				//httpSdk.HttpListen("0.0.0.0",8181);
				echo2File("test");
				//char strDateTime[32]; \
					//CUtils::getInstance()->cur_time_c(strDateTime); 
				//httpSdk.SendDataToClient(clientfd, retStatusCodeStr);
				nSend(clientfd, retStatusCodeStr.c_str(), retStatusCodeStr.size(), 0);

	}
	else if (ret == ENOTCONN) {
		//curfds--;
		NETWORK_SDK httpSdk;
		std::string retStatusCodeStr;
		httpSdk.SetHttpStatusCode(200, retStatusCodeStr);
		//httpSdk.HttpListen("0.0.0.0",8181);
		echo2File("test");
		//char strDateTime[32]; \
							//CUtils::getInstance()->cur_time_c(strDateTime); 
//httpSdk.SendDataToClient(clientfd, retStatusCodeStr);
		nSend(clientfd, retStatusCodeStr.c_str(), retStatusCodeStr.size(), 0);
		close(clientfd);
	}
	else {
		NETWORK_SDK httpSdk;
		std::string retStatusCodeStr;
		httpSdk.SetHttpStatusCode(200, retStatusCodeStr);
		//httpSdk.HttpListen("0.0.0.0",8181);
		echo2File("test");
		//char strDateTime[32]; \
							//CUtils::getInstance()->cur_time_c(strDateTime); 
//httpSdk.SendDataToClient(clientfd, retStatusCodeStr);
		nSend(clientfd, retStatusCodeStr.c_str(), retStatusCodeStr.size(), 0);

	}

	free(rClient);
	free(job);

}

void client_data_process(int clientfd) {

	char buffer[MAX_BUFFER];
	bzero(buffer, MAX_BUFFER);
	int length = 0;
	//int ret = nRecv(clientfd, buffer, MAX_BUFFER, &length);
	int ret = recv(clientfd, buffer, MAX_BUFFER, 0);
	if (length > 0) {
		if (nRun || buffer[0] == 'a') {
			printf(" TcpRecv --> curfds : %d, buffer: %s\n", curfds, buffer);

			//nSend(clientfd, buffer, strlen(buffer), 0);
			send(clientfd, buffer, strlen(buffer), 0);
		}

	}
	else if (ret == ENOTCONN) {
		curfds--;
		close(clientfd);
	}
	else {

	}

}



int listenfd(int fd, int *fds) {
	int i = 0;

	for (i = 0; i < MAX_PORT; i++) {
		if (fd == *(fds + i)) return *(fds + i);
	}
	return 0;
}

struct timeval tv_begin;
struct timeval tv_cur;

void *listen_thread(void *arg) {

	int i = 0;
	int epoll_fd = *(int *)arg;
	struct epoll_event events[MAX_EPOLLSIZE];


	gettimeofday(&tv_begin, NULL);

	while (1) {

		int nfds = epoll_wait(epoll_fd, events, curfds, 5);
		if (nfds == -1) {
			perror("epoll_wait");
			break;
		}
		for (i = 0; i < nfds; i++) {

			int sockfd = listenfd(events[i].data.fd, sockfds);
			if (sockfd) {
				struct sockaddr_in client_addr;
				memset(&client_addr, 0, sizeof(struct sockaddr_in));
				socklen_t client_len = sizeof(client_addr);

				int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
				if (clientfd < 0) {
					perror("accept");
					return NULL;
				}

				if (curfds++ > 1000 * 1000) {
					nRun = 1;
				}
#if 0
				printf(" Client %d: %d.%d.%d.%d:%d \n", curfds, *(unsigned char*)(&client_addr.sin_addr.s_addr), *((unsigned char*)(&client_addr.sin_addr.s_addr) + 1),
					*((unsigned char*)(&client_addr.sin_addr.s_addr) + 2), *((unsigned char*)(&client_addr.sin_addr.s_addr) + 3),
					client_addr.sin_port);
#elif 0
				if (curfds % 1000 == 999) {
					printf("connections: %d, fd: %d\n", curfds, clientfd);
				}
#else
				if (curfds % 1000 == 999) {

					memcpy(&tv_cur, &tv_begin, sizeof(struct timeval));

					gettimeofday(&tv_begin, NULL);
					int time_used = TIME_SUB_MS(tv_begin, tv_cur);

					printf("connections: %d, sockfd:%d, time_used:%d\n", curfds, clientfd, time_used);
				}
#endif
				ntySetNonblock(clientfd);
				ntySetReUseAddr(clientfd);

				struct epoll_event ev;
				ev.events = EPOLLIN | EPOLLET | EPOLLOUT;
				ev.data.fd = clientfd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientfd, &ev);

			}
			else {

				int clientfd = events[i].data.fd;
#if 1
				nRun = 0;
				if (nRun) {
					printf(" New Data is Comming\n");
					client_data_process(clientfd);
				}
				else {

					client_t *rClient = (client_t*)malloc(sizeof(client_t));
					memset(rClient, 0, sizeof(client_t));
					rClient->fd = clientfd;

					job_t *job = (job_t*)malloc(sizeof(job_t));
					job->job_function = client_job;
					job->user_data = rClient;
					workqueue_add_job(&workqueue, job);


				}
#else
				client_data_process(clientfd);
#endif
			}
		}

	}

}

int main(void) {
	int i = 0;

	printf("C1000K Server Start\n");
	NETWORK_SDK httpSdk;
	std::string retStatusCodeStr;
	httpSdk.SetHttpStatusCode(200, retStatusCodeStr);
	httpSdk.HttpListen("0.0.0.0",8181);
	echo2File("test");
	//char strDateTime[32]; \
						//CUtils::getInstance()->cur_time_c(strDateTime); 
    //httpSdk.SendDataToClient(clientfd, retStatusCodeStr);

	threadpool_init(); //


#if 0
	int epoll_fd = epoll_create(MAX_EPOLLSIZE);
#else

	int epoll_fds[CPU_CORES_SIZE] = { 0 };
	pthread_t thread_id[CPU_CORES_SIZE] = { 0 };

	for (i = 0; i < CPU_CORES_SIZE; i++) {
		epoll_fds[i] = epoll_create(MAX_EPOLLSIZE);

		pthread_create(&thread_id[i], NULL, listen_thread, &epoll_fds[i]);
	}


#endif
	for (i = 0; i < MAX_PORT; i++) {

		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			perror("socket");
			return 1;
		}

		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(struct sockaddr_in));

		addr.sin_family = AF_INET;
		addr.sin_port = htons(SERVER_PORT + i);
		addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) {
			perror("bind");
			return 2;
		}

		if (listen(sockfd, 5) < 0) {
			perror("listen");
			return 3;
		}

		sockfds[i] = sockfd;
		printf("C1000K Server Listen on Port:%d\n", SERVER_PORT + i);

		struct epoll_event ev;

		ev.events = EPOLLIN | EPOLLET; //EPOLLLT
		ev.data.fd = sockfd;
		epoll_ctl(epoll_fds[i%CPU_CORES_SIZE], EPOLL_CTL_ADD, sockfd, &ev);
	}

	for (i = 0; i < CPU_CORES_SIZE; i++) {
		pthread_join(thread_id[i], NULL);
	}


	getchar();
	printf("end\n");
}
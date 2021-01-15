#include "errno.h"
#include "fcntl.h"
#include "sys/epoll.h"
#include "iostream"
#include "stdlib.h"
#include "stdio.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "memory.h"
#include "execinfo.h"
#include "signal.h"
#include "sstream"
#include <sys/timeb.h>
#include <chrono>
#include "threadPool/threadPool.h"

#define BUFFER_LENGTH  1024
#define EPOLL_SIZE 1024
#define MAX_PORT 100

typedef struct {
	int epfd;
	int* socketfds;

}PROCESS_SOCK_REQUEST;

int isListenFds(int fd, int *fds)
{
	int i = 0;
	for (i = 0; i < MAX_PORT; i++)
	{
		if (fd == fds[i])
			return fd;
	}
	return 0;
}

int echo2File(std::string logStr)
{
	std::string shellCommand("echo ");
	shellCommand +="\""+ logStr+"\" > 1.txt";
	system(shellCommand.c_str());
}

int SetHttpStatusCode(int statusCode, std::string& retStatusCodeStr, std::string httpVersion = "1.1");
int SetHttpHeaders(std::string& httpHeaderKey, std::string& httpHeaderValue, std::string& retStatusCodeStr);

void *client_routine(void* arg)
{
	int clientfd = *(int*)arg;
	while (1)
	{
		char buffer[BUFFER_LENGTH] = { 0 };
		int len = recv(clientfd,buffer,BUFFER_LENGTH,0);
		if (len < 0)
		{
			close(clientfd);
			break;
		}
		else if (len = 0) {//disconnect
			close(clientfd);
			break;
		}
		else
		{
			printf("Recv:%s,%d bytes(s)\n",buffer,len);
		}
	}
}

std::mutex g_mutex;
int clientWork(int fd,struct epoll_event* events, PROCESS_SOCK_REQUEST& m_pSR)
{
	int iRet = 0;
	//std::lock_guard<std::mutex> m_lgmutex(g_mutex);
	int sockfd = isListenFds(fd, m_pSR.socketfds);
	printf("sockfd:%d", sockfd);
	echo2File("sockfd:"+std::to_string(sockfd)+"-fd:"+std::to_string(fd));
	if (fd == sockfd) //listen
	{
		printf("listen:%d", sockfd);
		struct sockaddr_in client_addr;
		memset(&client_addr, 0, sizeof(struct sockaddr_in));
		int reuse = 1;
		socklen_t client_len = sizeof(client_addr);
		fcntl(sockfd, F_SETFL, O_NONBLOCK);
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
		int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
		if (clientfd != -1)
		{
			fcntl(clientfd, F_SETFL, O_NONBLOCK);

			setsockopt(clientfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
			struct epoll_event ev;
			ev.events = EPOLLIN | EPOLLET;
			ev.data.fd = clientfd;
			epoll_ctl(m_pSR.epfd, EPOLL_CTL_ADD, clientfd, &ev);
			echo2File("accept finish");
		}
	}
	else
	{
		int clientfd = fd;
		char buffer[BUFFER_LENGTH] = { 0 };
		int len = 0,totalSize=0;
		while(len = recv(clientfd, buffer, BUFFER_LENGTH, 0)>0)
		{printf("nready:%d", len);
		totalSize += len;
		echo2File("nreadyLenth:" + std::to_string(len)+"totalSize:"+std::to_string(totalSize));
		
		{
			printf("Recv:%s,%d bytes(s),clientfd:%d\n", buffer, len, clientfd);
			memset(buffer, 0, sizeof(buffer));
			/*std::string statusCodeStr;
			
			close(clientfd);
			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd = clientfd;
			epoll_ctl(m_pSR.epfd, EPOLL_CTL_DEL, clientfd, &ev);*/
			//send(clientfd, statusCodeStr.c_str(), strlen(buffer), 0);
		}
		}
		printf("recvAfter:%d\n", len);
		std::string statusCodeStr;
		SetHttpStatusCode(200, statusCodeStr);
		//SetHttpHeaders();
		statusCodeStr += "\r\n\r\n";
		send(clientfd, statusCodeStr.c_str(), statusCodeStr.size(), 0);
		if (len < 0)
		{
			close(clientfd);
			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd = clientfd;
			epoll_ctl(m_pSR.epfd, EPOLL_CTL_DEL, clientfd, &ev);
		}
		else if (len == 0) //disconnect
		{
			close(clientfd);
			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd = clientfd;
			epoll_ctl(m_pSR.epfd, EPOLL_CTL_DEL, clientfd, &ev);
		}
		

	}
	//echo2File("clientWork finish");
	return iRet;
}


void *processListen(void* arg)
{
	printf("processListen enter");
	std::string shellCommand("echo ");
	shellCommand += "=====processListen enter===== > 1.txt";
	system(shellCommand.c_str());
	PROCESS_SOCK_REQUEST* m_pSR = (PROCESS_SOCK_REQUEST*)arg;
	struct epoll_event events[EPOLL_SIZE] = { 0 };
	ThreadPool m_threadPool(10);
	while (1)
	{
		int nready = -1;
		{
			//std::lock_guard<std::mutex> m_lgmtx(g_mutex);
		    nready = epoll_wait(m_pSR->epfd, events, EPOLL_SIZE, 5);
		}
		if (nready == -1) continue;

		int i = 0;
		for (i = 0; i < nready; i++)
		{
			//echo2File("nready:" + std::to_string(nready));
			//std::function<int(int, struct epoll_event8, PROCESS_SOCK_REQUEST&)> m_listenFunc = std::bind(&clientWork, i, events, m_pSR);
			int fd = events[i].data.fd;
			m_threadPool.enqueue(
				/*[fd,&events,m_pSR]() {
				clientWork(fd, events, *m_pSR);
			});*/
				clientWork, int(events[i].data.fd),std::ref(events),*m_pSR);
			//clientWork(fd,events,*m_pSR);
		}
	}
	return 0;
}

void processListen2(void* arg)
{
	printf("processListen22 enter");
	
	return ;
}

static void output_addrline(char addr[])
{
	char cmd[256];
	char line[256];
	char addrline[32] = { 0, };
	char *str1, *str2;
	FILE* file;

	str1 = strchr(addr, '[');
	str2 = strchr(addr, ']');
	if (str1 == NULL || str2 == NULL)
	{
		return;
	}
	memcpy(addrline, str1 + 1, str2 - str1);
	snprintf(cmd, sizeof(cmd), "addr2line -e /proc/%d/exe %s ", getpid(), addrline);
	file = popen(cmd, "r");
	if (NULL != fgets(line, 256, file))
	{
		printf("%s\n", line);
	}
	pclose(file);
}

void sigHandler(int signo)
{
	//LOG_ERROR_ARGS("=====recv SIGINT %d=====", signo);
	printf("=====recv SIGINT %d=====", signo);
	std::string shellCommand("echo ");
	shellCommand += "=====recv SIGINT %d=====" + std::to_string(signo)+"> 1.txt";
	system(shellCommand.c_str());
	//´òÓ¡´íÎó¶ÑÕ»ÐÅÏ¢
	//LOG_ERROR("----------------------------Dump Program Error Strings-------------------------");
	int j = 0, nptrs = 0;
	void* buffer[100] = { NULL };
	char** strings = NULL;
	nptrs = backtrace(buffer, 100);
	
	//LOG_ERROR_ARGS("backtrace() returned %d addresses", nptrs);
	printf("backtrace() returned %d addresses\n", nptrs);
	shellCommand.clear();
	shellCommand = "echo ";
	shellCommand += "backtrace() returned " + std::to_string(nptrs) + " addresses> 1.txt";
	system(shellCommand.c_str());
	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		//LOG_ERROR("backtrace_symbols null");
		printf("backtrace_symbols null");
		shellCommand.clear();
		shellCommand = "echo ";
		shellCommand += "backtrace_symbols null > 1.txt";
		system(shellCommand.c_str());
		//LOG_ERROR("-------------------------------------------------------------------------------");
		printf("-------------------------------------------------------------------------------");
		shellCommand.clear();
		shellCommand = "echo ";
		shellCommand += "------------------------------------------------------------------------------- > 1.txt";
		system(shellCommand.c_str());
		return;
	}
	for (j = 0; j < nptrs; j++) {
		//LOG_ERROR_ARGS("  [%02d] %s", j, strings[j]);
		printf("[%0d] %s", j, strings[j]);
		output_addrline(strings[j]);
		/*shellCommand.clear();
		shellCommand = "echo ";
		shellCommand += "["+std::to_string(j)+"]"+ strings[j] +"> 1.txt";
		system(shellCommand.c_str());*/
	}
	free(strings);
	//LOG_ERROR("-------------------------------------------------------------------------------");
	printf("-------------------------------------------------------------------------------");
	//»Ö¸´Ä¬ÈÏÐÅºÅ²Ù×÷
	signal(signo, SIG_DFL);
	raise(signo);
}


char* cur_time_c(char* strDateTime)
{
	struct timeb tp_cur;
	ftime(&tp_cur);

	struct tm btm;

#ifdef WIN32
	localtime_s(&btm, &tp_cur.time);
#else
	localtime_r(&tp_cur.time, &btm);
#endif

	sprintf(strDateTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
		btm.tm_year + 1900, btm.tm_mon + 1, btm.tm_mday,
		btm.tm_hour, btm.tm_min, btm.tm_sec, tp_cur.millitm);

	return strDateTime;
}

//char* cur_time_cpp11(char strDateTime[32])
//{
//	static const std::chrono::hours = EIGHT_HOURS(8);
//
//	auto nowLocalTimeCount
//		= std::chrono::system_clock::now().time_since_epoch()
//		+ EIGHT_HOURS;
//
//	std::chrono::hours now_h
//		= std::chrono::duration_cast<std::chrono::hours>(nowLocalTimeCount);
//	std::chrono::minutes now_m
//		= std::chrono::duration_cast<std::chrono::minutes>(nowLocalTimeCount);
//	std::chrono::seconds now_s
//		= std::chrono::duration_cast<std::chrono::seconds>(nowLocalTimeCount);
//	std::chrono::milliseconds now_ms
//		= std::chrono::duration_cast<std::chrono::milliseconds>(nowLocalTimeCount);
//
//	sprintf(strDateTime, "%02d:%02d:%02d.%03d",
//		now_h % 24, now_m % 60, now_s % 60, now_ms % 1000);
//
//	return strDateTime;
//}

int SetHttpStatusCode(int statusCode,std::string& retStatusCodeStr,std::string httpVersion)
{
	int iRet = 0;
	retStatusCodeStr += "HTTP/" + httpVersion;

	if (statusCode == 200)
	{
		retStatusCodeStr += " 200 OK";
	}
	return iRet;
}

int SetHttpHeaders(std::string& httpHeaderKey, std::string& httpHeaderValue, std::string& retStatusCodeStr)
{
	int iRet = 0;
	retStatusCodeStr += httpHeaderKey + ":" + httpHeaderValue;

	/*if (statusCode == 200)
	{
		retStatusCodeStr += " 200 OK";
	}*/
	return iRet;
}

int main(int argc,char *argv[])
{
	signal(SIGSEGV, sigHandler);
	signal(SIGABRT, sigHandler);

	char strDateTime[32];
	cur_time_c(strDateTime);
	printf("curdate:%s",strDateTime);
	if (argc < 2)
	{
		printf("Param Error\n");
		return -1;
	}
	unsigned int uA = 3214;
	int b = int(140243567116540);
	int port = atoi(argv[1]);
	int epfd = epoll_create(1);
	static int socketfds[MAX_PORT] = { 0 };
	int i = 0;
	for (i = 0; i < MAX_PORT; i++)
	{


		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port+i);
		addr.sin_addr.s_addr = INADDR_ANY;
		//fcntl(sockfd, F_SETFL, O_NONBLOCK);
		int reuse = 1;
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int));
		if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0)
		{
			perror("bind");
			return 2;
		}
		if (listen(sockfd, 5) < 0) {
			perror("listen");
			return 3;
		}
		socketfds[i] = sockfd;
		struct epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = sockfd;
		epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
	}

#if 0
	while (1)
	{
		struct sockaddr_in client_addr;
		memset(&client_addr,0,sizeof(struct sockaddr_in));
		socklen_t client_len = sizeof(client_addr);

		int clientfd = accept(sockfd,(struct sockaddr*)&client_addr,&client_len);

		pthread_t thread_id;
		pthread_create(&thread_id,NULL,client_routine,&clientfd);
	}
#else
	
	struct epoll_event events[EPOLL_SIZE] = { 0 };
	/*pthread_t thread_id;
	pthread_create(&thread_id, NULL, client_routine, &clientfd);*/
	PROCESS_SOCK_REQUEST m_pSR({ epfd ,socketfds});
	void * p = &(m_pSR);
	std::thread listenThread(processListen, p);
	listenThread.join();

	
#endif

}
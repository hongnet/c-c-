#ifndef _TCP_SERVER_DEFINE_H_
#define _TCP_SERVER_DEFINE_H_


//#if (defined _WIN32 || defined _WIN64)
//#ifndef CALLBACK
//#define CALLBACK __stdcall
//#endif
//
//#ifdef HTTP_EXPORTS
//#define SERVER_DECLARE(type) extern "C" __declspec(dllexport) type CALLBACK
//#else
//#define SERVER_DECLARE(type) extern "C" __declspec(dllimport) type  CALLBACK
//#endif
//#else
//#define SERVER_DECLARE(type) extern "C" type
//#endif


#define BUFFER_LENGTH  1024
#define EPOLL_SIZE 1024
#define MAX_PORT 1

#include "iostream"
#include "stdlib.h"
#include "stdio.h"
#if defined(__linux__)
#include "errno.h"
#include "fcntl.h"
#include "sys/epoll.h"
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
#endif
#include "threadPool/threadPool.h"
#include "map"
#include "Utils.h"
#include  "poll.h"

typedef struct {
	int epfd;
	int* socketfds;

}PROCESS_SOCK_REQUEST_STRUCT;

typedef struct {
	int fd;
	std::string data;
}RECV_DATA_STRUCT;

struct Con {
	std::string readed;
	size_t written;
	Con() : written(0) {}
};


typedef std::function<int (int, std::string)> RecvFromClientFunPtr;

#if (defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64))
#ifdef VSKSDKWIN_EXPORTS
#define NS_DEVNET_API extern "C"  __declspec(dllexport)
#else
#define NS_DEVNET_API extern "C"  __declspec(dllimport)
#endif

//#define NS_DEVNET_API extern "C"
#define CALL_METHOD __stdcall
#else
#define NS_DEVNET_API extern "C" 
#define CALL_METHOD 
#endif
typedef int (CALL_METHOD *RecvDB) (RECV_DATA_STRUCT& recvData,void* userData);

class NETWORK_SDK
{
public:
	 NETWORK_SDK();
	 ~NETWORK_SDK();
	 int HttpListen(std::string ip="0.0.0.0", int port=8080);
	 int SendDataToClient(int clientFd,std::string data);
	 int RecvDataFromClient(int clientFd, std::string data);
	 int SetRecvRegisterCB(RecvDB recvFromClientFun,void* userData);
	 int SetHttpStatusCode(int statusCode, std::string& retStatusCodeStr, std::string httpVersion="1.1");
	 int SetHttpHeaders(std::string& httpHeaderKey, std::string& httpHeaderValue, std::string& retStatusCodeStr);
private:
	int processListen(void* arg);
	int clientWork(int fd, struct epoll_event events, PROCESS_SOCK_REQUEST_STRUCT& m_pSR);
	int updateEvents(int efd, int fd, int events, int op);
	int handleAccept(int efd, int fd);
	int sendRes(int efd, int fd, std::string httpRes = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 1048576\r\n\r\n123456");
	int handleRead(int efd, int fd);
	int handleWrite(int efd, int fd);
	//RecvFromClientFunPtr m_recvFromClientCallBackPtr;
	RecvDB m_recvFromClientCallBackPtr;
	void * m_userData;
	
	std::map<int, Con> cons;
};

#endif


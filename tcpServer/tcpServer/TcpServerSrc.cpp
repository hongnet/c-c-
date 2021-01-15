#include "TcpServer.h"
#include"Utils.h"


FILE* logFile;
ThreadPool m_threadPool(10);
#define exit_if(r, ...)                                                                          \
    if (r) {                                                                                     \
        fprintf(logFile,__VA_ARGS__);                                                                     \
        fprintf(logFile,"%s:%d error no: %d error msg %s\n", __FILE__, __LINE__, errno, strerror(errno)); \
        return 1;                                                                                 \
    }

std::map<int, bool> fdIsRun;
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

int setNonBlock(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	exit_if(flags < 0, "fcntl failed");
	int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	exit_if(r < 0, "fcntl failed");
}

int NETWORK_SDK::updateEvents(int efd, int fd, int events, int op) {
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = events;
	ev.data.fd = fd;
	fprintf(logFile, "%s fd %d events read %d write %d\n", op == EPOLL_CTL_MOD ? "mod" : "add", fd, ev.events & EPOLLIN, ev.events & EPOLLOUT);
	int r = epoll_ctl(efd, op, fd, &ev);
	exit_if(r, "epoll_ctl failed");
}

int NETWORK_SDK::handleAccept(int efd, int fd) {
	struct sockaddr_in raddr;
	socklen_t rsz = sizeof(raddr);
	int cfd = accept(fd, (struct sockaddr *) &raddr, &rsz);
	exit_if(cfd < 0, "accept failed");
	sockaddr_in peer, local;
	socklen_t alen = sizeof(peer);
	int r = getpeername(cfd, (sockaddr *)&peer, &alen);
	exit_if(r < 0, "getpeername failed");
	fprintf(logFile, "accept a connection from %s\n", inet_ntoa(raddr.sin_addr));
	setNonBlock(cfd);
	updateEvents(efd, cfd, EPOLLIN | EPOLLOUT | EPOLLET, EPOLL_CTL_ADD);
}
int NETWORK_SDK::sendRes(int efd, int fd,std::string httpRes) {
	Con &con = cons[fd];
	struct pollfd pollfds;
	pollfds.fd = fd;
	pollfds.events = (POLLIN | POLLERR | POLLHUP);
	int res = poll(&pollfds, 1, 1000);
	fprintf(logFile, "res:%d,fd:%d", res, fd);
	if (pollfds.revents & POLLHUP) {
		//ret_code = ENOTCONN;
		fprintf(logFile, "POLLHUP,fd:%d", fd);
	}
	else if (pollfds.revents & POLLIN)
	{
		fprintf(logFile, "POLLIN,fd:%d", fd);
		return 1;
	}
	if (res < 0) {
		if (errno == EINTR) {
			//continue;
		}
		//ret_code = (errno != 0 ? errno : EINTR);
	}
	else if (res == 0) {
		//ret_code = ETIMEDOUT;
		//break;
	}
	if (!con.readed.length())
		return 1;
	size_t left = httpRes.length() - con.written;
	int wd = 0;
	while ((wd = ::write(fd, httpRes.data() + con.written, left)) > 0) {
		con.written += wd;
		left -= wd;
		fprintf(logFile, "write %d bytes left: %lu\n", wd, left);
	};
	if (left == 0) {
		//        close(fd); // 测试中使用了keepalive，因此不关闭连接。连接会在read事件中关闭
		updateEvents(efd, fd, EPOLLIN, EPOLL_CTL_MOD); //当所有数据发送结束后，不再关注其缓冲区可写事件
		cons.erase(fd);
		return 1;
	}
	fprintf(logFile, "write for %d: %d %s,wd:%d\n", fd, errno, strerror(errno), wd);
	if (wd < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
	{
		updateEvents(efd, fd, EPOLLIN | EPOLLOUT, EPOLL_CTL_MOD);
		return 1;
	}
	if (wd <= 0) {
		fprintf(logFile, "write error for %d: %d %s\n", fd, errno, strerror(errno));
		close(fd);
		cons.erase(fd);
	}
}

int NETWORK_SDK::handleRead(int efd, int fd) {

	char buf[4096];
	int n = 0;
	while ((n = ::read(fd, buf, sizeof buf)) > 0) {
		fprintf(logFile, "read %d bytes,length:%d\n", n, cons[fd].readed.length());
		std::string &readed = cons[fd].readed;
		readed.append(buf, n);
		if (readed.length() > 4) {
			if (readed.substr(readed.length() - 2, 2) == "\n\n" || readed.substr(readed.length() - 4, 4) == "\r\n\r\n") {
				//当读取到一个完整的http请求，测试发送响应
				sendRes(efd, fd);
			}
		}
	}
	fprintf(logFile, "read for %d: %d %s,readrtn:%d\n", fd, errno, strerror(errno), n);
	if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
	{
		updateEvents(efd, fd, EPOLLIN | EPOLLOUT, EPOLL_CTL_MOD);
		return 1;
	}
	//实际应用中，n<0应当检查各类错误，如EINTR
	if (n < 0) {
		fprintf(logFile, "read %d error: %d %s\n", fd, errno, strerror(errno));
	}
	close(fd);
	cons.erase(fd);
}

int NETWORK_SDK::handleWrite(int efd, int fd) {
	sendRes(efd, fd);
}

int NETWORK_SDK::clientWork(int fd, struct epoll_event events, PROCESS_SOCK_REQUEST_STRUCT& m_pSR)
{
	int iRet = 0;
	//std::lock_guard<std::mutex> m_lgmutex(g_mutex);
	int sockfd = isListenFds(fd, m_pSR.socketfds);
	fprintf(logFile,"sockfd:%d", sockfd);
	if (events.events & (EPOLLIN | EPOLLERR)) {
		if (fd == sockfd) {
			handleAccept(m_pSR.epfd, fd);
		}
		else {
			handleRead(m_pSR.epfd, fd);
		}
	}
	else if (events.events & EPOLLOUT) { // 请注意，例子为了保持简洁性，没有很好的处理极端情况，例如EPOLLIN和EPOLLOUT同时到达的情况
		fprintf(logFile, "handling epollout\n");
		handleWrite(m_pSR.epfd, fd);
	}
	else {
		exit_if(1, "unknown event");
	}
	return iRet;
}
int NETWORK_SDK::processListen(void* arg)
{
	echo2File("=====processListen enter=====");
	PROCESS_SOCK_REQUEST_STRUCT* m_pSR = (PROCESS_SOCK_REQUEST_STRUCT*)arg;
	struct epoll_event events[EPOLL_SIZE] = { 0 };
	
	while (1)
	{
		int nready = -1;
		{
			//std::lock_guard<std::mutex> m_lgmtx(g_mutex);
			nready = epoll_wait(m_pSR->epfd, events, EPOLL_SIZE, 10000);
		}
		if (nready == -1) continue;

		int i = 0;
		for (i = 0; i < nready; i++)
		{
			int fd = events[i].data.fd;
			clientWork(int(events[i].data.fd), events[i], *m_pSR);
			//m_threadPool.enqueue(
				//&NETWORK_SDK::clientWork,this, int(events[i].data.fd), events[i], *m_pSR);
		}
	}
	
	return 0;
}

int NETWORK_SDK::SetHttpStatusCode(int statusCode, std::string& retStatusCodeStr, std::string httpVersion)
{
	int iRet = 0;
	retStatusCodeStr += "HTTP/" + httpVersion;

	if (statusCode == 200)
	{
		retStatusCodeStr += " 200 OK\r\n\r\n";
	}
	return iRet;
}

int NETWORK_SDK::SetHttpHeaders(std::string& httpHeaderKey, std::string& httpHeaderValue, std::string& retStatusCodeStr)
{
	int iRet = 0;
	retStatusCodeStr += httpHeaderKey + ":" + httpHeaderValue;

	/*if (statusCode == 200)
	{
	retStatusCodeStr += " 200 OK";
	}*/
	return iRet;
}
NETWORK_SDK::NETWORK_SDK()
	:m_recvFromClientCallBackPtr(NULL)
{
	fdIsRun.clear();
}
NETWORK_SDK::~NETWORK_SDK()
{

}
int NETWORK_SDK::HttpListen(std::string ip, int port)
{
	logFile = fopen("httpOut.txt", "a+");
	//freopen("out.txt", "a+", stdout);
	setbuf(logFile, NULL);
	::signal(SIGPIPE, SIG_IGN);
	int epfd = epoll_create(1);
	exit_if(epfd < 0, "epoll_create failed");
	static int socketfds[MAX_PORT] = { 0 };
	int i = 0;
	for (i = 0; i < MAX_PORT; i++)
	{

		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port + i);
		//addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_addr.s_addr =inet_addr(ip.c_str());
		fcntl(sockfd, F_SETFL, O_NONBLOCK);
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
		//updateEvents(epfd, sockfd, EPOLLIN, EPOLL_CTL_ADD);
		socketfds[i] = sockfd;
		struct epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = sockfd;
		epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
	}
	PROCESS_SOCK_REQUEST_STRUCT m_pSR({ epfd ,socketfds });
	void * p = &(m_pSR);
	std::thread listenThread(&NETWORK_SDK::processListen,this, p);
	listenThread.join();

}

int NETWORK_SDK::SendDataToClient(int clientFd, std::string data)
{
	int reuse = 1;
	fcntl(clientFd, F_SETFL, O_NONBLOCK);
	setsockopt(clientFd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
	send(clientFd, data.c_str(), data.size(), 0);
}

int NETWORK_SDK::RecvDataFromClient(int clientFd, std::string data)
{
	int reuse = 1;
	char buffer[BUFFER_LENGTH] = { 0 };
	fcntl(clientFd, F_SETFL, O_NONBLOCK);
	setsockopt(clientFd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(bool));
	int len = 0, totalSize = 0;
	while (len = recv(clientFd, buffer, BUFFER_LENGTH, 0)>0)
	{
		printf("nready:%d", len);
		totalSize += len;
		echo2File("nreadyLenth:" + std::to_string(len) + "totalSize:" + std::to_string(totalSize));
		{
			printf("Recv:%s,%d bytes(s),clientfd:%d\n", buffer, len, clientFd);
			memset(buffer, 0, sizeof(buffer));
		}
	}
	echo2File("recvAfter:"+std::to_string(len));
}

int NETWORK_SDK::SetRecvRegisterCB(RecvDB recvFromClientFun, void * userData)
{
	m_recvFromClientCallBackPtr = recvFromClientFun;
	m_userData = userData;
	return 0;
}

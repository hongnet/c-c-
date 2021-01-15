/*
* ���룺c++ -o epoll-et epoll-et.cc
* ���У� ./epoll-et
* ���ԣ�curl -v localhost
* �ͻ��˷���GET����󣬷���������1M�����ݣ��ᴥ��EPOLLOUT����epoll-et�������־����EPOLLOUT�¼��õ�����ȷ�Ĵ���
*/
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <string>
#include "Utils.h"
#include "poll.h"
using namespace std;

bool output_log = true;
FILE* logFile = fopen("httpOut.txt", "a+");

#define exit_if(r, ...)                                                                          \
    if (r) {                                                                                     \
        fprintf(logFile,__VA_ARGS__);                                                                     \
        fprintf(logFile,"%s:%d error no: %d error msg %s\n", __FILE__, __LINE__, errno, strerror(errno)); \
        exit(1);                                                                                 \
    }

void setNonBlock(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	exit_if(flags < 0, "fcntl failed");
	int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	exit_if(r < 0, "fcntl failed");
}

void updateEvents(int efd, int fd, int events, int op) {
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = events;
	ev.data.fd = fd;
	fprintf(logFile,"%s fd %d events read %d write %d\n", op == EPOLL_CTL_MOD ? "mod" : "add", fd, ev.events & EPOLLIN, ev.events & EPOLLOUT);
	int r = epoll_ctl(efd, op, fd, &ev);
	exit_if(r, "epoll_ctl failed");
}

void handleAccept(int efd, int fd) {
	struct sockaddr_in raddr;
	socklen_t rsz = sizeof(raddr);
	int cfd = accept(fd, (struct sockaddr *) &raddr, &rsz);
	exit_if(cfd < 0, "accept failed");
	sockaddr_in peer, local;
	socklen_t alen = sizeof(peer);
	int r = getpeername(cfd, (sockaddr *)&peer, &alen);
	exit_if(r < 0, "getpeername failed");
	fprintf(logFile,"accept a connection from %s\n", inet_ntoa(raddr.sin_addr));
	setNonBlock(cfd);
	updateEvents(efd, cfd, EPOLLIN | EPOLLOUT | EPOLLET, EPOLL_CTL_ADD);
}
struct Con {
	string readed;
	size_t written;
	Con() : written(0) {}
};
map<int, Con> cons;

string httpRes;
void sendRes(int efd,int fd) {
	Con &con = cons[fd];
	struct pollfd pollfds;
	pollfds.fd = fd;
	pollfds.events = (POLLIN | POLLERR | POLLHUP);
	int res = poll(&pollfds, 1, 1000);
	fprintf(logFile,"res:%d,fd:%d", res, fd);
	if (pollfds.revents & POLLHUP) {
		//ret_code = ENOTCONN;
		fprintf(logFile,"POLLHUP,fd:%d", fd);
	}
	else if(pollfds.revents & POLLIN)
	{
		fprintf(logFile,"POLLIN,fd:%d", fd);
		return;
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
		return;
	size_t left = httpRes.length() - con.written;
	int wd = 0;
	while ((wd = ::write(fd, httpRes.data() + con.written, left)) > 0) {
		con.written += wd;
		left -= wd;
		if (output_log)
			fprintf(logFile,"write %d bytes left: %lu\n", wd, left);
	};
	if (left == 0) {
		//        close(fd); // ������ʹ����keepalive����˲��ر����ӡ����ӻ���read�¼��йر�
		updateEvents(efd, fd, EPOLLIN, EPOLL_CTL_MOD); //���������ݷ��ͽ����󣬲��ٹ�ע�仺������д�¼�
		cons.erase(fd);
		return;
	}
	fprintf(logFile,"write for %d: %d %s,wd:%d\n", fd, errno, strerror(errno),wd);
	if (wd < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
	{
		updateEvents(efd, fd, EPOLLIN | EPOLLOUT, EPOLL_CTL_MOD);
		return;
	}
	if (wd <= 0) {
		fprintf(logFile,"write error for %d: %d %s\n", fd, errno, strerror(errno));
		close(fd);
		cons.erase(fd);
	}
}

void handleRead(int efd, int fd) {

	char buf[4096];
	int n = 0;
	while ((n = ::read(fd, buf, sizeof buf)) > 0) {
		if (output_log)
			fprintf(logFile,"read %d bytes,length:%d\n", n, cons[fd].readed.length());
		string &readed = cons[fd].readed;
		readed.append(buf, n);
		if (readed.length() > 4) {
			if (readed.substr(readed.length() - 2, 2) == "\n\n" || readed.substr(readed.length() - 4, 4) == "\r\n\r\n") {
				//����ȡ��һ��������http���󣬲��Է�����Ӧ
				sendRes(efd,fd);
			}
		}
	}
	fprintf(logFile,"read for %d: %d %s,readrtn:%d\n", fd, errno, strerror(errno), n);
	if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
	{
		updateEvents(efd, fd, EPOLLIN | EPOLLOUT, EPOLL_CTL_MOD);
		return;
	}
	//ʵ��Ӧ���У�n<0Ӧ�������������EINTR
	if (n < 0) {
		fprintf(logFile,"read %d error: %d %s\n", fd, errno, strerror(errno));
	}
	close(fd);
	cons.erase(fd);
}

void handleWrite(int efd, int fd) {
	sendRes(efd,fd);
}

void loop_once(int efd, int lfd, int waitms) {
	const int kMaxEvents = 20;
	struct epoll_event activeEvs[100];
	int n = epoll_wait(efd, activeEvs, kMaxEvents, waitms);
	if (output_log)
		fprintf(logFile,"epoll_wait return %d\n", n);
	for (int i = 0; i < n; i++) {
		int fd = activeEvs[i].data.fd;
		int events = activeEvs[i].events;
		if (events & (EPOLLIN | EPOLLERR)) {
			if (fd == lfd) {
				handleAccept(efd, fd);
			}
			else {
				handleRead(efd, fd);
			}
		}
		else if (events & EPOLLOUT) { // ��ע�⣬����Ϊ�˱��ּ���ԣ�û�кܺõĴ��������������EPOLLIN��EPOLLOUTͬʱ��������
			if (output_log)
				fprintf(logFile,"handling epollout\n");
			handleWrite(efd, fd);
		}
		else {
			exit_if(1, "unknown event");
		}
	}
}

int main(int argc, const char *argv[]) {
	
	//freopen("out.txt", "a+", stdout);
	setbuf(logFile, NULL);
	if (argc > 1) {
		output_log = false;
	}
	::signal(SIGPIPE, SIG_IGN);
	httpRes = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 1048576\r\n\r\n123456";
	for (int i = 0; i < 1048570; i++) {
		httpRes += '\0';
	}
	unsigned short port = 80;
	int epollfd = epoll_create(1);
	exit_if(epollfd < 0, "epoll_create failed");
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	setNonBlock(listenfd);
	int reuse = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int));
	exit_if(listenfd < 0, "socket failed");
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	int r = ::bind(listenfd, (struct sockaddr *) &addr, sizeof(struct sockaddr));
	exit_if(r, "bind to 0.0.0.0:%d failed %d %s", port, errno, strerror(errno));
	r = listen(listenfd, 20);
	exit_if(r, "listen failed %d %s", errno, strerror(errno));
	fprintf(logFile,"fd %d listening at %d\n", listenfd, port);

	updateEvents(epollfd, listenfd, EPOLLIN, EPOLL_CTL_ADD);
	
	for (;;) {  //ʵ��Ӧ��Ӧ��ע���źŴ��������˳�ʱ������Դ
		loop_once(epollfd, listenfd, 10000);
	}
	return 0;
}
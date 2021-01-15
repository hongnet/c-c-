// WinIOCPServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "iostream"
#include "windows.h"
#include "WinSock2.h"
#include "ws2ipdef.h"
#include "Utils.h"
//
typedef SOCKET socket_type;
#if defined(_WIN32) || defined(__CYGWIN__)
typedef SOCKET socket_type;
const SOCKET invalid_socket = INVALID_SOCKET;
typedef sockaddr socket_addr_type;

#endif
int register_handle(
	HANDLE handle, int ec)
{
	int concurrency_hint = -1;
	HANDLE handle2 = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0,
		static_cast<DWORD>(concurrency_hint >= 0 ? concurrency_hint : DWORD(~0)));
	if (::CreateIoCompletionPort(handle, handle2, 0, 0) == 0)
	{
		DWORD last_error = ::GetLastError();
		/*ec = boost::system::error_code(last_error,
			boost::asio::error::get_system_category());*/
	}
	else
	{
		//ec = boost::system::error_code();
	}
	return ec;
}
int main()
{
	sockaddr base;
#if defined(_WIN32) || defined(__CYGWIN__)
//23,1,6
	socket_type s = ::WSASocketW(AF_INET6, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	//get_last_error(ec, s == invalid_socket);
	if (s == invalid_socket)
		return s;

	//if (af == BOOST_ASIO_OS_DEF(AF_INET6))
	{
		// Try to enable the POSIX default behaviour of having IPV6_V6ONLY set to
		// false. This will only succeed on Windows Vista and later versions of
		// Windows, where a dual-stack IPv4/v6 implementation is available.
		DWORD optval = 0;
		::setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
			reinterpret_cast<const char*>(&optval), sizeof(optval));
	}
	HANDLE sock_as_handle = reinterpret_cast<HANDLE>(s);
	if (register_handle(sock_as_handle, 0))
		echo2File("error registerHandle");
	int reuse = 1;
	int addrlen;
	if (AF_INET)
		addrlen= sizeof(sockaddr_in);
	else
		addrlen= sizeof(sockaddr_in6);
	::setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
		(const char*)&reuse, sizeof(reuse));
	socket_addr_type  sockaddrin;
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(8097);
	sockAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	::bind(s, (socket_addr_type *)&sockAddr, addrlen);
	int result = ::listen(s, SOMAXCONN);

	return s;
#endif
}

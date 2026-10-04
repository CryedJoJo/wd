/**
 * Project Untitled
 */

#include "InetAddress.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
/**
 * InetAddress implementation
 */

/**
 * @param ip
 * @param port
 */
InetAddress::InetAddress(const string &ip, unsigned short port)
{
	::bzero(&addr_, sizeof(struct sockaddr_in)); //加的
	addr_.sin_family      = AF_INET;
	addr_.sin_addr.s_addr = ::inet_addr(ip.c_str());
	addr_.sin_port        = ::htons(port);
}

/**
 * @param addr
 */
InetAddress::InetAddress(const struct sockaddr_in &addr)
    : addr_(addr)
{
}

InetAddress::~InetAddress()
{
}

/**
 * @return string
 */
string InetAddress::getIP()
{
	return string(inet_ntoa(addr_.sin_addr));
}

/**
 * @return int
 */
int InetAddress::getPort()
{
	return ::ntohs(addr_.sin_port);
}

/**
 * @return struct sockarr_in*
 */
struct sockaddr_in *InetAddress::getInetAddrPtr()
{
	return &addr_;
}
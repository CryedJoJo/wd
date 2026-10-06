/**
 * Project Untitled
 */

#include "InetAddress.h"

#include <arpa/inet.h>

InetAddress::InetAddress(const string &ip, int port)
{
	addr_.sin_family      = AF_INET;
	// addr_.sin_addr.s_addr = htonl(atoi(ip.c_str())); 乱写的，所以bind失败 bind: Cannot assign requested address
	addr_.sin_addr.s_addr = ::inet_addr(ip.c_str());
	addr_.sin_port        = htons(port);
}

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

/**
 * Project Untitled
 */

#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include <sys/types.h>
#include <netinet/in.h>

#include <string>

using std::string;

class InetAddress {

public:
	InetAddress(const string &ip, int port);
	InetAddress(const struct sockaddr_in &addr);
	~InetAddress();
	string getIP();

	int getPort();

	struct sockaddr_in *getInetAddrPtr();

private:
	/* data */
	struct sockaddr_in addr_;
};

#endif //_INETADDRESS_H
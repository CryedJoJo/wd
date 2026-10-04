/**
 * Project Untitled
 */


#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include <string>
#include <netinet/in.h> //socketaddr_in

using std::string;

class InetAddress {
public: 
    
/**
 * @param ip
 * @param port
 */
 InetAddress(const string& ip, unsigned short port);
    
/**
 * @param addr
 */
 InetAddress(const struct sockaddr_in & addr);
    
 ~InetAddress();
    
string getIP();
    
int getPort();
    
struct sockaddr_in* getInetAddrPtr();
private: 
    struct sockaddr_in addr_;
};

#endif //_INETADDRESS_H
#include "TcpConnection.h"
#include "Acceptor.h"

void test(){

	Acceptor acceptor("127.0.0.1", 8888);
	TcpConnection con(acceptor.accept());
	
    while (1)
    {
		string msg = con.receive();
		con.send(msg);
		/* code */
	}
}

int main(){

	test();
	return 0;
}
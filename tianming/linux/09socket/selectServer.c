#include <stdio.h>
#include <string.h>
#include <unistd.h> //read write close STDIN_FILENO
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <arpa/inet.h>  //转换函数
#include <netinet/in.h> //socketaddr_in

typedef struct connection{
	int fd;
	int alive;
}con;

int
main()
{

	//socket
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	//address and convert
	struct sockaddr_in socketaddr;
	socketaddr.sin_family = AF_INET;
	socketaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	socketaddr.sin_port        = htons(atoi("8080"));

    //bind
	bind(socket_fd, (const struct sockaddr *)&socketaddr, sizeof(socketaddr));
	//listen
	listen(socket_fd, 10);

	//set
	fd_set set;

	//set init
	FD_ZERO(&set);

    //FD_SET
	FD_SET(socket_fd, &set);

	int conn[1024];
	memset(conn, 0, sizeof(conn));
	// int index = 0;

	while(1) {
		// copy set
		fd_set temp;
		memcpy(&temp, &set, sizeof(set));

		//select
		int size = select(10, &temp, NULL, NULL, NULL);

		//handle fd
		for(int i = 0; i < size; ++i){
			//new connection
			if(FD_ISSET(socket_fd, &set)) {
				//accept
				int con_fd = accept(socket_fd, (struct sockaddr *)&socketaddr, 0);

				//FD_SET
				FD_SET(con_fd, &set);
				conn[con_fd] = 1;
			}

			for()
            if(FD_ISSET()) {
                //msg

                //reverse and submit
            }
		}
	}

	//close

	return 0;
}
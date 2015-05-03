#include "tcp_server.h"
#include <iostream>

tcp_server::tcp_server()
{
}


tcp_server::~tcp_server() 
{
	if (sharedServer)
		sharedServer = NULL;
}

tcp_server* tcp_server::getInstance()
{
	if (!sharedServer) {
		sharedServer = new tcp_server();
		sharedServer->init();
	}
	return sharedServer;
}

void tcp_server::init()
{
	_listenfd = socket (AF_INET , SOCK_STREAM , 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = 9477;
	bind(_listenfd , (struct sockaddr*)&server_addr , sizeof(server_addr));

	FD_ZERO (&rset);
	FD_ZERO (&allset);
	FD_SET  (_listenfd , &allset);
	for ( int i = 0 ; i < FD_SETSIZE ; ++i)
		client[i] = -1;
	maxfd = _listenfd;
}

void tcp_server::run()
{
	socklen_t clilen = sizeof (client_addr);
	listen (_listenfd , 5);
	while (true) {
		rset = allset;
		auto nready = select(maxfd , &rset , NULL , NULL , NULL);
		auto i = 0;
		if ( FD_ISSET (_listenfd  , &rset) ) {
			_connfd = accept (_listenfd , (struct sockaddr*)&client_addr , &clilen);		

			for (; i < FD_SETSIZE ; ++i) {
				if (client[i] == -1) {
					client[i] = _connfd;
					break;
				}
			}
			if ( i == FD_SETSIZE ) {
				throw runtime_error("too many clients");
				return ;
			}
			FD_SET (_connfd , &allset);
			maxfd = max (maxfd , _connfd);

			if (--nready <= 0)
				continue;
		}

		for (i = 0 ; i < maxfd ; ++i) {
			if (client[i] == -1)	
				continue;
			if (FD_ISSET(client[i] , &rset)) {
				if (int n = read (client[i] , buf , MAXLINE) == 0) {
					close (client[i]);
					FD_CLR(client[i] , &allset);
					client[i] = -1;
				}
				else
					write (client[i] , buf , n);
			}
			if (--nready <= 0)
				break;
		}
		
	}
}

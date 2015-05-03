#include "tcp_client.h"

tcpClient::~tcpClient()
{
	if (sharedClient)
		sharedClient = NULL;
}

tcpClient* tcpClient::getInstance()
{
	if (!sharedClient) {
		sharedClient = new tcpClient();
		sharedClient->init();	
	}
	return sharedClient;
}

void tcpClient::init()
{
	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	server_addr.sin_family		= AF_INET;
	server_addr.sin_addr.s_addr	= inet_addr("127.0.0.1");	
	server_addr.sin_port		= 9477;
}

void tcpClient::run(FILE* fp = stdin)
{	
	connect (sockfd , (struct sockaddr*)&server_addr ,(socklen_t) sizeof(server_addr));
	fd_set rset;
	fd_set allset;
	FD_ZERO (&allset);
	FD_SET  (sockfd , &allset);
	FD_SET  (fileno(fp) , &allset);
	auto maxfd	= max(fileno(fp) , sockfd) + 1;
	auto stdineof	= 0;
	while (true) {
		rset = allset;
		select (maxfd , &rset , NULL ,NULL ,NULL);
		if (FD_ISSET (sockfd , &rset)) {
			auto n = read (sockfd , buf , sizeof(buf));
			if ( n == 0 ) {
				if (stdineof == 1)
					return ;
				else
					throw runtime_error("server terminaled");
			}
			cout << buf << endl;
		}
		
		if (FD_ISSET (fileno(fp) , &rset)) {
			auto n = read (fileno(fp) , buf , MAXLINE);
			if ( n == 0 ) {
				stdineof = 1;
				shutdown (sockfd , SHUT_WR);
				FD_CLR (fileno(fp) , &allset);
				continue;
			}
			write (sockfd , buf , n);
		}
	}
}

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>
#include <stdexcept>

using namespace std;
const int  MAXLINE = 4096;

class tcpClient {
public:
	tcpClient () = default;
	tcpClient (const tcpClient &) = delete;
	tcpClient& operator = (const tcpClient&) = delete;
	~tcpClient ();
	void run(FILE* fp = stdin);
	static tcpClient* getInstance();

private:
	void init();


private:
	struct sockaddr_in server_addr;
	int	sockfd;
	char	buf[MAXLINE];
	static	tcpClient* sharedClient;
};



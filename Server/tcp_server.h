#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdexcept>

using namespace std;
const	int MAXLINE = 4096;


class tcp_server {
public:
	tcp_server();
	tcp_server(tcp_server &rhs) = delete ;
	tcp_server& operator = (const tcp_server &rhs) = delete;

	~tcp_server();

	void init();
	static tcp_server* getInstance();
	void run() ;

private:
	int	_listenfd;
	int	_listenport;
	int	_connfd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	static tcp_server *sharedServer;
	
	fd_set rset , allset;
	int 	maxfd;
	int	client[FD_SETSIZE];
	char	buf[MAXLINE];
	
};


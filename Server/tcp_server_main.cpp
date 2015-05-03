#include <iostream>
#include "tcp_server.h"

tcp_server* tcp_server::sharedServer = NULL;

int main(void)
{
	tcp_server *pser   = tcp_server::getInstance();
	pser->run();
	return 0;
}

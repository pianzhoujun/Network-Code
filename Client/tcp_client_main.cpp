#include "tcp_client.h"

tcpClient* tcpClient::sharedClient = NULL;

int main(void)
{
	tcpClient *client = tcpClient::getInstance();
	client->run();
	return 0;
}

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "Socket.h"

using namespace std;

void Socket::Send_Data(const char *data, int port_number){
	int server;
	int portNum = port_number;
	bool isExit = false;
	int bufsize = strlen(data);
    char const *ip = "127.0.0.1";

	struct sockaddr_in server_addr;

	server = socket(AF_INET, SOCK_STREAM, 0);

	if(server < 0)
	{
		cout << "Error creating socket..." << endl;
		exit(1);
	}

	cout << "Client Socket created." << endl;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNum);

	if(connect(server, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0)
		cout << "Connecting to server..." << endl;

	cout << "Connection confirmed" << endl;

	send(server, data, bufsize, 0);

	cout << "Connection terminated..." << endl;
	cout << "Goodbye" << endl;

	close(server);

	return ;
}

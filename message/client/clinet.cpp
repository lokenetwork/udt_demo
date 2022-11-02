#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include <iostream>
#include "udt.h"

using namespace std;

int main(int argc, char* argv[])
{
	int res;
	char* server_ip = argv[1];
	char* server_port = argv[2];
	UDTSOCKET client;
	struct addrinfo hints;
	struct addrinfo* local_info;
	struct addrinfo* server_info;
	char data_send[100] = "It is a client message";
	char data_recv[100] = { 0 };

	if ((3 != argc) || (0 == atoi(argv[2])))
	{
		cout << "usage: client server_ip server_port" << endl;
		return 1;
	}

	//初始化 UDT 库
	UDT::startup();

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	if (0 != getaddrinfo(NULL, "", &hints, &local_info))
	{
		cout << "incorrect network address.\n" << endl;
		return 1;
	}
	client = UDT::socket(local_info->ai_family, local_info->ai_socktype, local_info->ai_protocol);
	freeaddrinfo(local_info);

	if (0 != getaddrinfo(server_ip, server_port, &hints, &server_info))
	{
		cout << "incorrect server/peer address. " << server_ip << ":" << server_port << endl;
		return 1;
	}
	//连接服务器，隐性绑定。
	if (UDT::ERROR == UDT::connect(client, server_info->ai_addr, server_info->ai_addrlen))
	{
		cout << "connect: " << UDT::getlasterror().getErrorMessage() << endl;
		return 1;
	}
	freeaddrinfo(server_info);
	cout << "connect server " << server_ip << " success" << endl;

	res = UDT::sendmsg(client, data_send, sizeof(data_send), -1, false);
	if (UDT::ERROR == res)
	{
		cout << "send:" << UDT::getlasterror().getErrorMessage() << endl;
		return 1;
	}
	cout << "send data success" << endl;
	//休眠 1 秒再读取数据
	Sleep(1000);
	res = UDT::recvmsg(client, data_recv, sizeof(data_recv));
	if (UDT::ERROR == res)
	{
		cout << "recv:" << UDT::getlasterror().getErrorMessage() << endl;
		return 0;
	}
	cout << "recv:" << data_recv << endl;

	Sleep(5000);
	UDT::close(client);
	//释放 UDT 库
	UDT::cleanup();
	return 0;
}

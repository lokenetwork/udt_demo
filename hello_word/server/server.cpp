#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include <iostream>
#include "udt.h"

using namespace std;

int main(int argc, char* argv[])
{
	//UDT �������ľ��
	UDTSOCKET serv;
	//UDT �ͻ��˾��
	UDTSOCKET client;
	addrinfo hints;
	addrinfo* info;
	//������ udp �˿�
	char port[] = "9000";
	//�ͻ�����Ϣ
	sockaddr_storage client_addr;
	int addrlen = sizeof(client_addr);
	char data[100] = {0};

	//��ʼ�� UDT ��
	UDT::startup();
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_socktype = SOCK_DGRAM;
	if (0 != getaddrinfo(NULL, port, &hints, &info))
	{
		cout << "illegal port number or port is busy.\n" << endl;
		return 0;
	}
	serv = UDT::socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (UDT::ERROR == UDT::bind(serv, info->ai_addr, info->ai_addrlen))
	{
		cout << "bind: " << UDT::getlasterror().getErrorMessage() << endl;
		return 0;
	}
	freeaddrinfo(info);
	if (UDT::ERROR == UDT::listen(serv, 10))
	{
		cout << "listen: " << UDT::getlasterror().getErrorMessage() << endl;
		return 0;
	}
	cout << "server is listen at port: " << port << endl;

	client = UDT::accept(serv, (sockaddr*)&client_addr, &addrlen);
	if (UDT::INVALID_SOCK == client)
	{
		cout << "accept: " << UDT::getlasterror().getErrorMessage() << endl;
		return 0;
	}
	if (UDT::ERROR == UDT::recv(client, data, 100, 0))
	{
		cout << "recv:" << UDT::getlasterror().getErrorMessage() << endl;
		return 0;
	}
	cout << "recv:" << data << endl;

	//���� 5 �롣

	UDT::close(client);
	UDT::close(serv);
	//�ͷ� UDT ��
	UDT::cleanup();
	return 0;
}
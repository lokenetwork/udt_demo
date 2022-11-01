#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include <iostream>
#include "udt.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "ENOSERVER : " << CUDTException::ENOSERVER << endl;
	return 0;
}

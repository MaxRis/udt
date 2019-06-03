#include "commonapp.h"

#include <iostream>

using namespace std;

void printInputParams(int argc, char* argv[], int offset)
{
	cout << "--- Input arguments ---" << endl;
	cout << "UDT_MSS = " << argv[UDT_MSS_PARAM + offset] << endl;
	cout << "UDT_SNDBUF = " << argv[UDT_SNDBUF_PARAM + offset] << endl;
	cout << "UDT_RCVBUF = " << argv[UDT_RCVBUF_PARAM + offset] << endl;
	cout << "UDP_SNDBUF = " << argv[UDP_SNDBUF_PARAM + offset] << endl;
	cout << "UDPT_RCVBUF = " << argv[UDP_RCVBUF_PARAM + offset] << endl;
}

void setInputParams(UDTSOCKET socket, int argc, char* argv[], int offset)
{
   UDT::setsockopt(socket, 0, UDT_MSS, new int(atoi(argv[UDT_MSS_PARAM + offset])), sizeof(int));
   UDT::setsockopt(socket, 0, UDT_SNDBUF, new int(atoi(argv[UDT_SNDBUF_PARAM + offset])), sizeof(int));
   UDT::setsockopt(socket, 0, UDT_RCVBUF, new int(atoi(argv[UDT_RCVBUF_PARAM + offset])), sizeof(int));
   UDT::setsockopt(socket, 0, UDP_SNDBUF, new int(atoi(argv[UDT_SNDBUF_PARAM + offset])), sizeof(int));
   UDT::setsockopt(socket, 0, UDP_RCVBUF, new int(atoi(argv[UDT_RCVBUF_PARAM + offset])), sizeof(int));
}

void printAppliedParams(UDTSOCKET socket)
{
	cout << "--- Applied UDP params ---" << endl;
	int value = 0;
	int valueSize = sizeof(int);
	UDT::getsockopt(socket, 0, UDT_MSS, &value, &valueSize);
	cout << "Applied UDT_MSS = " << value << endl;
	UDT::getsockopt(socket, 0, UDT_SNDBUF, &value, &valueSize);
	cout << "Applied UDT_SNDBUF = " << value << endl;
	UDT::getsockopt(socket, 0, UDT_RCVBUF, &value, &valueSize);
	cout << "Applied UDT_RCVBUF = " << value << endl;
	UDT::getsockopt(socket, 0, UDP_SNDBUF, &value, &valueSize);
	cout << "Applied UDP_SNDBUF = " << value << endl;
	UDT::getsockopt(socket, 0, UDP_RCVBUF, &value, &valueSize);
	cout << "Applied UDP_RCVBUF = " << value << endl;
}
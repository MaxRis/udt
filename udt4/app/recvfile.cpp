#ifndef WIN32
   #include <arpa/inet.h>
   #include <netdb.h>
#else
   #include <winsock2.h>
   #include <ws2tcpip.h>
#endif
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <udt.h>

using namespace std;

int main(int argc, char* argv[])
{
   if ((argc != 9) || (0 == atoi(argv[2])))
   {
      cout << "usage: recvfile server_ip server_port remote_filename local_filename UDT_MSS UDT_SNDBUF UDT_RCVBUF UDT_MAXBW" << endl;
      return -1;
   }

   const int UDT_MSS_PARAM = 5;
   cout << "UDT_MSS = " << argv[UDT_MSS_PARAM] << endl;
   const int UDT_SNDBUF_PARAM = 6;
   cout << "UDT_SNDBUF = " << argv[UDT_SNDBUF_PARAM] << endl;
   const int UDT_RCVBUF_PARAM = 7;
   cout << "UDT_RCVBUF = " << argv[UDT_RCVBUF_PARAM] << endl;
   const int UDT_MAXBW_PARAM = 8;
   cout << "UDT_MAXBW = " << argv[UDT_MAXBW_PARAM] << endl;

   // use this function to initialize the UDT library
   UDT::startup();

   struct addrinfo hints, *peer;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_flags = AI_PASSIVE;
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;

   UDTSOCKET fhandle = UDT::socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);

   UDT::setsockopt(fhandle, 0, UDT_MSS, new int(atoi(argv[UDT_MSS_PARAM])), sizeof(int));
   UDT::setsockopt(fhandle, 0, UDT_SNDBUF, new int(atoi(argv[UDT_SNDBUF_PARAM])), sizeof(int));
   UDT::setsockopt(fhandle, 0, UDT_RCVBUF, new int(atoi(argv[UDT_RCVBUF_PARAM])), sizeof(int));
   UDT::setsockopt(fhandle, 0, UDT_MAXBW, new int(atoi(argv[UDT_MAXBW_PARAM])), sizeof(int));

   if (0 != getaddrinfo(argv[1], argv[2], &hints, &peer))
   {
      cout << "incorrect server/peer address. " << argv[1] << ":" << argv[2] << endl;
      return -1;
   }

   // connect to the server, implict bind
   if (UDT::ERROR == UDT::connect(fhandle, peer->ai_addr, peer->ai_addrlen))
   {
      cout << "connect: " << UDT::getlasterror().getErrorMessage() << endl;
      return -1;
   }

   freeaddrinfo(peer);


   // send name information of the requested file
   int len = strlen(argv[3]);

   if (UDT::ERROR == UDT::send(fhandle, (char*)&len, sizeof(int), 0))
   {
      cout << "send: " << UDT::getlasterror().getErrorMessage() << endl;
      return -1;
   }

   if (UDT::ERROR == UDT::send(fhandle, argv[3], len, 0))
   {
      cout << "send: " << UDT::getlasterror().getErrorMessage() << endl;
      return -1;
   }

   // get size information
   int64_t size;

   if (UDT::ERROR == UDT::recv(fhandle, (char*)&size, sizeof(int64_t), 0))
   {
      cout << "send: " << UDT::getlasterror().getErrorMessage() << endl;
      return -1;
   }

   if (size < 0)
   {
      cout << "no such file " << argv[3] << " on the server\n";
      return -1;
   }

   // receive the file
   fstream ofs(argv[4], ios::out | ios::binary | ios::trunc);
   int64_t recvsize; 
   int64_t offset = 0;

   if (UDT::ERROR == (recvsize = UDT::recvfile(fhandle, ofs, offset, size)))
   {
      cout << "recvfile: " << UDT::getlasterror().getErrorMessage() << endl;
      return -1;
   }

   UDT::close(fhandle);

   ofs.close();

   // use this function to release the UDT library
   UDT::cleanup();

   return 0;
}

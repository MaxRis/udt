#ifndef _COMMON_APP_H_
#define _COMMON_APP_H_

#include "udt.h"

#define UDT_MSS_PARAM 0
#define UDT_SNDBUF_PARAM 1
#define UDT_RCVBUF_PARAM 2
#define UDP_SNDBUF_PARAM 3
#define UDP_RCVBUF_PARAM 4

void printInputParams(int argc, char* argv[], int offset);
void setInputParams(UDTSOCKET socket, int argc, char* argv[], int offset);
void printAppliedParams(UDTSOCKET socket);


#endif // _COMMON_APP_H_

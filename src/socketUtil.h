#ifndef SOCKET_UTIL_
#define SOCKET_UTIL_

#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <stdbool.h>

#define PORT 5000
#define MAX_LINE 4096
#define S_A struct sockaddr

int createSocket();
struct sockaddr_in* createAddress(char* IP);
void exit_Error(const char *format, ...);
char *bin2hex(const unsigned char *input, size_t len);

#endif 
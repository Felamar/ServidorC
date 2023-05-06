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
#include <unistd.h>

#define PORT 5000
#define MAX_LINE 4096
#define S_A struct sockaddr

typedef struct movie {
  char *title;
  char *description;
  showtime showtimes[4];
} movie;

typedef struct showtime {
  char *time;
  bool seats[2][5];
} showtime;

void sendMovieData(movie movies[], int client_fd);
void handleRequest(int client_fd, char *request);
void serveFile(int client_fd, char *filename);
char* getMimeType(char *filename);
void sendResponse(int client_fd, char *response);
char* decodeURL(char *url);

#endif 
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
#include <ncurses.h>
#include <time.h>

#define PORT 5000
#define MAX_LINE 4096
#define S_A struct sockaddr

typedef struct showtime {
  char *time;
  bool seats[10];
} showtime;

typedef struct movie {
  char *title;
  char *description;
  showtime showtimes[4];
} movie;

void sendMovieData(movie movies[], int client_fd, int n_movies);
void serveFile(int client_fd, char *filename);
char* getMimeType(char *filename);
void sendResponse(int client_fd, char *response);
char* decodeURL(char *url);
char *movieToJSON(movie m);
void assignShowTimes(movie *movies, showtime *showtimes);
char *showtimeToJSON();
void setSeatsValues(char **details, int client_fd, movie* movies);
#endif 
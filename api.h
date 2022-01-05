#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "macros.h"

//break up Url

int getProtocol(int i, char **argv,char *protocol);
int getUser(int i,char **argv,char *name);
int getPassword(int i,char **argv,char *password);
int getHost(int i,char **argv,char *host);
int getPathFile(int i,char **argv,char *path_file);
int getFileName(char *path_file,char *file_name);

//client
void printPassive(char *response3);
void read_answer(int socket, char *host_answer);
int parseResponse(char* response);
void readConnection(int sockfd, char *connection); 
void create_file(int sockfd_file_transfer, char* path_file);
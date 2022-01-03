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

int getProtocol(int i, char **argv,char *protocol);
int getUser(int i,char **argv,char *name);
int getPassword(int i,char **argv,char *password);
int getHost(int i,char **argv,char *host);
int getPathFile(int i,char **argv,char *path_file);
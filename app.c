#include "api.h"
#include "macros.h"

int main(int argc, char** argv){
    //nº de argumentos

    if (argc < 2) {
        perror("Por favor envie um URL\n");
        return 1;
    } else if (argc > 2) {
        perror("Demasiados argumentos\n");
        return 1;
    }

    //Variaveis

    char protocol[SIZE];
    memset(protocol, 0, SIZE);

    char name[SIZE];
    memset(name, 0, SIZE);

    char password[SIZE];
    memset(password, 0, SIZE);

    char host[SIZE];
    memset(host, 0, SIZE);

    char path_file[SIZE];
    memset(path_file, 0, SIZE);

    char file_name[SIZE];
    memset(file_name, 0, SIZE);

    //break up url

    if( getProtocol(0,argv,protocol)==1){
        perror("invalid Protocol\n");
        return 1;
    }


    if( getUser(strlen(protocol)+4,argv,name)==1){
        perror("invalid User\n");
        return 1;
    }

    int aux = getPassword(strlen(protocol)+4+strlen(name)+1,argv,password);
    switch(aux){
        case 0:{
            break;
        }
        case 1:{
            perror("invalid Password\n");
            return 1;
        }
        case 2:{
            perror("invalid Path, need to be something like this ftp://[<user>:<password>@]<host>/<url-path>\n");
            return 1;
        }
    }

    if( getHost(strlen(protocol)+4+strlen(name)+1+strlen(password)+2,argv,host)==1){
        perror("invalid Host\n");
        return 1;
    }
    
    getPathFile(strlen(protocol)+4+strlen(name)+1+strlen(password)+2+strlen(host)+1,argv,path_file);

    printf("Protocol: %s\n", protocol);
    printf("Name: %s\n", name);
    printf("Password: %s\n", password);
    printf("Host: %s\n", host);
    printf("PathFile: %s\n", path_file);

    struct hostent *h;

    if ((h = gethostbyname(host)) == NULL) {
        herror("gethostbyname()");
        exit(-1);
    }

    char* ip = inet_ntoa(*((struct in_addr *) h->h_addr));
    printf("IP Address : %s\n", ip);
}
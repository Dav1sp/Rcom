#include "api.h"


int getProtocol(int i, char **argv,char *protocol){
    int indice=0;
    while(i < strlen(argv[1])){
        if(argv[1][i]==':'&& argv[1][i+1]=='/'&&argv[1][i+2]=='/'&&argv[1][i+3]=='['){
            return 0;
        }
        else{
            protocol[indice]=argv[1][i];
            indice++;
        }
        i++;
    }
    return 1;
}

int getUser(int i,char **argv,char *name){
    int indice=0;
    while(i < strlen(argv[1])){
        if(argv[1][i]==':'){
            return 0;
        }
        else{
            name[indice]=argv[1][i];
            indice++;
        }
        i++;
    }
    return 1;
}
int getPassword(int i,char **argv,char *password){
    int indice=0;
    while(i < strlen(argv[1])){
        if(argv[1][i]=='@'&&argv[1][i+1]==']'){
            return 0;
        }
        else if(argv[1][i]=='@'&&argv[1][i+1]!=']'){
            return 2;
        }
        else{
            password[indice]=argv[1][i];
            indice++;
        }
        i++;
    }
    return 1;
}
int getHost(int i,char **argv,char *host){
    int indice=0;
    while(i < strlen(argv[1])){
        if(argv[1][i]=='/'){
            return 0;
        }
        else{
            host[indice]=argv[1][i];
            indice++;
        }
        i++;
    }
    return 1;
}
int getPathFile(int i,char **argv,char *path_file){
    int indice=0;
    while(i < strlen(argv[1])){
        path_file[indice]=argv[1][i];
        indice++;
        i++;
    }
    return 0;
}
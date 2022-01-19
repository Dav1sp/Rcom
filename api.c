#include "api.h"

//break up Url

int getProtocol(int i, char **argv,char *protocol){
    int indice=0;
    while(i < strlen(argv[1])){
        if(argv[1][i]==':'){
            if(argv[1][i+1]=='/'&&argv[1][i+2]=='/'&&argv[1][i+3]=='['){
                return 0;
            }
            else{
                return 1;
            }
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

int getFileName(char *path_file,char *file_name){
    int nBarras = 0;
    for(int i = 0; i < strlen(path_file); i++){
        if(path_file[i] == '/'){
            nBarras++;
        }
    }
    int nBarrasAtual = 0;
    int aux = 0;
    for(int i = 0; i < strlen(path_file); i++){
        if(nBarrasAtual == nBarras){
            file_name[aux] = path_file[i];
            aux++;
        }
        else if(path_file[i] == '/'){
            nBarrasAtual++;
        }
    }
    return 0;
}

//client
void printPassive(char *response3){
    int aux = 0;
            int aux1 = 0;
            printf("> Entering Passive Mode ");
            while(aux < strlen(response3)){
                if(aux1 == 0){
                    if(response3[aux] == '('){
                        printf("%c",response3[aux]);
                        aux1 = 1;
                        aux++;
                    }
                    else{
                        aux++;
                    }
                }
                else if(aux1==1){
                    if(response3[aux] == ')'){
                        printf("%c",response3[aux]);
                        aux++;
                        aux1=2;
                    }
                    else{
                        printf("%c",response3[aux]);
                        aux++;
                    }
                }
                else{
                    printf(".\n");
                    break;
                }
            }
}

int parseResponse(char* res){

    char bigSignificance[5];
	memset(bigSignificance, 0, 5);
	char smallSignificance[4];
	memset(smallSignificance, 0, 5);

    int ms = 0;
    int ls = 0;
    int i = 0;
    int state = 0;
    int number_comma = 0;

    while(i < strlen(res)){
        if(state == 0){
            if(res[i] == '('){
                state = 1;
                i++;
            }
            else{
                i++;
            }
        }
        else if(state == 1){
            if(number_comma < 4){
                if(res[i] == ','){
                    number_comma++;
                }
                i++;
            }
            else if(number_comma == 4){
                if(res[i] == ','){
                    number_comma++;
                }
                else{
                    bigSignificance[ms] = res[i];
                    ms++;
                }
                i++;
            }
            else if(number_comma == 5){
                if(res[i] == ')'){
                    state = 2;;
                }
                else{
                    smallSignificance[ls] = res[i];
                    ls++;
                }
                i++;
            }
        }
        else{
            i++;
        }

    }

   
    int theMostSignificant = atoi(bigSignificance);
	int theLessSignificant = atoi(smallSignificance);
	return (theMostSignificant * 256 + theLessSignificant);
}

void create_file(int sockfd_file_transfer, char* path){
	FILE *file = fopen(path, "wb+");

 	int bytes;
    int counter = 0;
    char buf[1000];

	printf("> Começar Download\n");

    while ((bytes = read(sockfd_file_transfer, buf, 1000))>0) {
            printf(".....");
            bytes = fwrite(buf, bytes, 1, file);   
    }
   
    fclose(file);
    printf("\n");
	printf("> Terminado!\n");
}

//Reading code that server responsed
void readConnection(int sockfd, char *connection){

    //Initializing variables
    int error = 0;
    int state = 0; 
	char c;

	while (state != 3){	
		read(sockfd, &c, 1);
		printf("%c", c);
		switch (state){
		case 0:
			if (c == ' '){
				if (error != 3){
					printf(" > Erro\n");
					return;
				}
				error = 0;
				state = 1;
			}
			else{
				if (c == '-'){
					state = 2;
					error=0;
				}
				else{
					if (isdigit(c)){
						connection[error] = c;
						error++;
					}
				}
			}
			break;
		case 1:
			if (c == '\n'){
				state = 3;
			}
			break;
		case 2:
			if(c == connection[error]){
				error++;
			}
			else{
				if(error== 3 && c == ' '){
				    state = 1;
				}
				else {
				    if(error==3 && c=='-'){
					    error=0;
				    }
				}	
			}
			break;
		}
	}
}

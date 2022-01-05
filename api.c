#include "api.h"

//break up Url

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



void read_answer(int socket, char *host_answer){

    bool stop = false;
    char response;
    int estado = 0;
    int i = 0;

    while(!stop){
        //ler do socket um char de cada vez
        read(socket, &response, 1);
        //imprime na consola a resposta do user
		printf("%c", response);
        
        switch(estado){
            case 0:
                if(i == 3){
                    estado = 1;
                }
                else{
                    host_answer[i] = response;
                    i++;
                }

                break;
            case 1:
                if(response == '\n'){
                    stop = true;
                }
                break;
        }
    }
}

int parseResponse(char* response){
    char mostSig[5];
	memset(mostSig, 0, 5);
	char lessSig[4];
	memset(lessSig, 0, 5);

    int i = 0;
    int ms = 0;
    int ls = 0;
    int state = 0;
    int number_comma = 0;

    while(i < strlen(response)){
        if(state == 0){
            if(response[i] == '('){
                state = 1;
                i++;
            }
            else{
                i++;
            }
        }
        else if(state == 1){
            if(number_comma < 4){
                if(response[i] == ','){
                    number_comma++;
                }
                i++;
            }
            else if(number_comma == 4){
                if(response[i] == ','){
                    number_comma++;
                }
                else{
                    mostSig[ms] = response[i];
                    ms++;
                }
                i++;
            }
            else if(number_comma == 5){
                if(response[i] == ')'){
                    state = 2;;
                }
                else{
                    lessSig[ls] = response[i];
                    ls++;
                }
                i++;
            }
        }
        else{
            i++;
        }

    }

   
    int mostSignificant = atoi(mostSig);
	int lessSignificant = atoi(lessSig);
	return (mostSignificant * 256 + lessSignificant);
}





void create_file(int sockfd_file_transfer, char* path_file){
	FILE *file = fopen(path_file, "wb+");

    
    char buffer[1000];
 	int bytes;
    int counter = 0;

	printf("> Starting download!\n");

    while ((bytes = read(sockfd_file_transfer, buffer, 1000))>0) {
            printf("...");
            bytes = fwrite(buffer, bytes, 1, file);   
    }
   
    fclose(file);
    printf("\n");
	printf("> Done!\n");

}

void readConnection(int sockfd, char *connection){
    int error = 0;
	char c;
	int state = 0; 
	while (state != 3){	
		read(sockfd, &c, 1);
		printf("%c", c);
		switch (state){
		case 0:
			if (c == ' '){
				if (error != 3){
					printf(" > Error\n");
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
#include "api.h"
#include "macros.h"

int main(int argc, char** argv){
    //nº de argumentos

    if (argc < 2) {
        perror("Por favor envie um URL\n");
        exit(-1);
    } else if (argc > 2) {
        perror("Demasiados argumentos\n");
        exit(-1);
    }

    //Variaveis

    char protocol[SIZE];
    memset(protocol, 0, SIZE);

    char name[SIZE];
    memset(name, 0, SIZE);
	
    char path_file[SIZE];
    memset(path_file, 0, SIZE);
	
    char file_name[SIZE];
    memset(file_name, 0, SIZE);

    char password[SIZE];
    memset(password, 0, SIZE);

    char host[SIZE];
    memset(host, 0, SIZE);



    //break up url

    if( getProtocol(0,argv,protocol)==1){
        perror("invalid Protocol\n");
        exit(-1);
    }


    if( getUser(strlen(protocol)+4,argv,name)==1){
        perror("invalid User\n");
        exit(-1);
    }

    int aux = getPassword(strlen(protocol)+4+strlen(name)+1,argv,password);
    switch(aux){
        case 0:{
            break;
        }
        case 1:{
            perror("invalid Password\n");
            exit(-1);
        }
        case 2:{
            perror("invalid Path, need to be something like this ftp://[<user>:<password>@]<host>/<url-path>\n");
            exit(-1);
        }
    }

    if( getHost(strlen(protocol)+4+strlen(name)+1+strlen(password)+2,argv,host)==1){
        perror("invalid Host\n");
        exit(-1);
    }
    
    getPathFile(strlen(protocol)+4+strlen(name)+1+strlen(password)+2+strlen(host)+1,argv,path_file);

    getFileName(path_file,file_name);

    printf("Protocol: %s\n", protocol);
    printf("Name: %s\n", name);
    printf("Password: %s\n", password);
    printf("Host: %s\n", host);
    printf("PathFile: %s\n", path_file);
    printf("FileName: %s\n", file_name);


    //get Ip address

    struct hostent *h;

    if ((h = gethostbyname(host)) == NULL) {
        herror("gethostbyname()");
        exit(-1);
    }

    char* ip = inet_ntoa(*((struct in_addr *) h->h_addr));
    printf("IP Address : %s\n", ip);


    //client

    int	sockfd; //socket de ligação
    int sockfd_file_transfer; //socker de transferencia de ficheiro
    struct	sockaddr_in server_addr;
	struct	sockaddr_in server_addr_file_transfer;

    bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(SERVER_PORT);  /*server TCP port must be network byte ordered */

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    /*connect to the server*/
    if (connect(sockfd,(struct sockaddr *) &server_addr,sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }

    char connection[3];
    readConnection(sockfd, connection);
    if (connection[0] == '2'){										 
        printf("> Connection Estabilished\n"); 
    }

    char response[SIZE];
    dprintf(sockfd, "user %s\r\n", name);
    read(sockfd, response, SIZE);
    printf("> Sending user \n");
    printf("%s\n",name);
    if(strncmp(response, "331 Please specify the password",31) == 0){//user certo,  mandar pass
        printf("> Sending password \n");
        printf("%s\n",password);
        char response2[SIZE];
        dprintf(sockfd, "pass %s\r\n", password);
        read(sockfd, response2, SIZE);
        if(strncmp(response2, "230", 3) == 0){// se pass certa dowload ficheiro(entrar em modo passivo e abrir 2º socket)
            //entrar em modo passivo e fazer download do ficheiro
	        dprintf(sockfd, "pasv\r\n");//printf para um filedescriptor
            char response3[SIZE];
            read(sockfd, response3, SIZE);
            printPassive(response3);

            int port_to_download = parseResponse(response3);
            printf("Port: %d\n", port_to_download);

            /*server address handling*/
            bzero((char*)&server_addr_file_transfer,sizeof(server_addr_file_transfer));
            server_addr_file_transfer.sin_family = AF_INET;
            server_addr_file_transfer.sin_addr.s_addr = inet_addr(ip);	        /*32 bit Internet address network byte ordered*/
            server_addr_file_transfer.sin_port = htons(port_to_download);		/*server TCP port must be network byte ordered */

            /*open an TCP socket*/
            if ((sockfd_file_transfer = socket(AF_INET,SOCK_STREAM,0)) < 0) {
                    perror("socket()");
                    exit(0);
                }
            /*connect to the server*/
            if(connect(sockfd_file_transfer, (struct sockaddr *)&server_addr_file_transfer, sizeof(server_addr_file_transfer)) < 0){
                    perror("connect()");
                    exit(0);
            }
            printf("> Sending retrieve\n");
	        dprintf(sockfd, "retr %s\r\n", path_file);//printf para um filedescriptor(antigo)
            create_file(sockfd_file_transfer, file_name);
            
        }
        else if(strncmp(response2, "430", 3) == 0){
            printf("> Invalid credentials \n");
            return -1;
        }
        else{
            printf("> Error occured2 \n");
            return -1;
        }
    }
    else{
        printf("> Error \n");
        printf("> %s\n",response);
        return -1;
    }

    close(sockfd_file_transfer);
	close(sockfd);

    return 0;
}

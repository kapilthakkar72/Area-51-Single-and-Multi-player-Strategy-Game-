#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string>
#include <ifaddrs.h>
#include <arpa/inet.h>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/**
This function will return my IPv4 IP Address
*/
string getMyIPAdress()
{
	struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    int i=0;
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
           
            if (i==1)
            {
              return addressBuffer;
            }
             i++;
        } 
    }

    return NULL;
}

int connectToServer(int port,const char serverIP[], string message)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    /*if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }*/
    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(serverIP);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
	error("ERROR connecting");    
    printf("Please enter the message: ");
    bzero(buffer,256);
   /* fgets(buffer,255,stdin);
    fputs(message.c_str(),buffer);
    n = write(sockfd,buffer,strlen(buffer));*/

    n = sendto(sockfd, message.c_str(), sizeof(message)*message.length(), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr)); 

    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}

int main(int argc, char *argv[])
{
	string message = "join game " + getMyIPAdress() + " 1234";
	int portno = atoi(argv[2]);
	char *serverIP=argv[1];
	connectToServer(portno,serverIP,message);
}
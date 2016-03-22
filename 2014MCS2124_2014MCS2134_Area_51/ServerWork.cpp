/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <algorithm>

using namespace std;

int noOfClients;

string client_1_ip;
string client_2_ip;
string client_3_ip;

int client_1_port;
int client_2_port;
int client_3_port;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/**
This function will split the string into multiple strings by making use of delimiter
*/
void SplitString(string message,string delimiter,string result[])
{
  message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
  string token, mystring(message);
  int i=0;
  while(token != mystring){
  token = mystring.substr(0,mystring.find_first_of(delimiter));
  mystring = mystring.substr(mystring.find_first_of(delimiter) + 1);
  
  result[i]=token;
  i++;
  }
}

void processClientIPandPort(string joinMessage)
{
  if(noOfClients!=3){
     string result[4];
     SplitString(joinMessage," ",result);

     switch(noOfClients)
     {
      case 0:
        {
          client_1_ip = result[2];
          client_1_port = atoi(result[3].c_str());
          noOfClients++;
          break;
        }
      case 1:
        {
          client_2_ip = result[2];
          client_2_port = atoi(result[3].c_str());
          noOfClients++;
          break;
        }
      case 2:
        {
          client_3_ip = result[2];
          client_3_port = atoi(result[3].c_str());
          noOfClients++;
          break;
        }

     }
  }
   
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     while(noOfClients !=3)
     {
       clilen = sizeof(cli_addr);
       newsockfd = accept(sockfd, 
                   (struct sockaddr *) &cli_addr, 
                   &clilen);
       if (newsockfd < 0) 
            error("ERROR on accept");
       bzero(buffer,256);
       n = read(newsockfd,buffer,255);
       if (n < 0) error("ERROR reading from socket");
       printf("Here is the message: %s\n",buffer);

       string message_recieved = string(buffer);
       processClientIPandPort(message_recieved);

       string send_ack = "Thank You for Joining ... " + message_recieved;

       n = write(newsockfd,send_ack.c_str(),50);
       if (n < 0) error("ERROR writing to socket");
    }

    printf("Client 1: %s %d\n",client_1_ip.c_str(),client_1_port );
    printf("Client 2: %s %d\n",client_2_ip.c_str(),client_2_port );
    printf("Client 3: %s %d\n",client_3_ip.c_str(),client_3_port );

   close(newsockfd);
   close(sockfd);
     return 0; 
}
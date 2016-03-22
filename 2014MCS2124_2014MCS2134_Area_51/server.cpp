int noOfClients = 0;

void SplitString(string message, string delimiter, string result[])
{
	message.erase(std::remove(message.begin(), message.end(), '\n'),message.end());
	string token , mystring(message);
	int i = 0;
	while (token != mystring)
	{
		token = mystring.substr(0, mystring.find_first_of(delimiter));
		mystring = mystring.substr(mystring.find_first_of(delimiter) + 1);

		result[i] = token;
		i++;
	}
}

string getMyIPAdress()
{
	struct ifaddrs * ifAddrStruct = NULL;
	struct ifaddrs * ifa = NULL;
	void * tmpAddrPtr = NULL;

	getifaddrs(&ifAddrStruct);

	int i = 0;
	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (!ifa->ifa_addr)
		{
			continue;
		}
		if (ifa->ifa_addr->sa_family == AF_INET)
		{ // check it is IP4
			// is a valid IP4 Address
			tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

			if (i == 1)
			{
				return addressBuffer;
			}
			i++;
		}
	}
	printf("Returning null from get ipaddress");
	return NULL;
}

/* Convert integer to string */

string convertIntToString(int i)
{
	char charString[155];
	snprintf(charString, 155, "%d", i);
	return string(charString);
}

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int processClientIPandPort(string joinMessage)
{
	if (noOfClients < 4)
	{
		string result[4];
		SplitString(joinMessage, ";", result);

		int teamNumber = atoi(result[2].c_str());
		int heroNumber = atoi(result[3].c_str());

		if (teamNumber == 1)
		{
			if (game.team1.noOfplayers == 0)
			{
				game.team1.player1.ipAddress = result[1];
				game.team1.player1.heroID = heroNumber;
				game.team1.player1.playerNumber = 1;
				game.team1.player1.xPosition = 70;
				game.team1.player1.yPosition = 850;
				game.team1.player1.teamNumber=1;
				game.team1.noOfplayers++;
				noOfClients++;
			}
			else if (game.team1.noOfplayers == 1)
			{
				game.team1.player2.ipAddress = result[1];
				game.team1.player2.heroID = heroNumber;
				game.team1.player2.playerNumber = 2;
				game.team1.player2.xPosition = 50;
				game.team1.player2.yPosition = 830;
				game.team1.player2.teamNumber=1;
				game.team1.noOfplayers++;
				noOfClients++;
			}
			else
			{
				printf("Team size exceeded for Team 1");
				return 0;
			}

		}
		else if (teamNumber == 2)
		{
			if (game.team2.noOfplayers == 0)
			{
				game.team2.player1.ipAddress = result[1];
				game.team2.player1.heroID = heroNumber;
				game.team2.player1.playerNumber = 1;
				game.team2.player1.xPosition = 1350;
				game.team2.player1.yPosition = 75;
				game.team2.player1.teamNumber=2;
				game.team2.noOfplayers++;
				noOfClients++;
			}
			else if (game.team2.noOfplayers == 1)
			{
				game.team2.player2.ipAddress = result[1];
				game.team2.player2.heroID = heroNumber;
				game.team2.player2.playerNumber = 2;
				game.team2.player2.xPosition = 1315;
				game.team2.player2.yPosition = 50;
				game.team2.player2.teamNumber=2;
				game.team2.noOfplayers++;
				noOfClients++;
			}
			else
			{
				printf("Team size exceeded for Team 2");
				return 0;
			}

		}
		else
		{
			printf("Invalid Team Number");
			return 0;
		}
		return 1;
	}
	printf("Number of clients exceeded");
	return 0;
}

int listenGameRequest()
{
	int sockfd , newsockfd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr , cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
		return 0;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("ERROR on binding");
		return 0;
	}
	listen(sockfd, 10);
	while (noOfClients != 4)
	{
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
		{
			error("ERROR on accept");
			return 0;
		}
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0)
		{
			error("ERROR reading from socket");
			return 0;
		}
		printf("Here is the message: %s\n", buffer);

		string message_recieved = string(buffer);
		processClientIPandPort(message_recieved);

		string send_ack = "Thank You for Joining ... " + message_recieved;

		n = write(newsockfd, send_ack.c_str(), 50);
		if (n < 0)
		{
			error("ERROR writing to socket");
			return 0;
		}
	}

	close(newsockfd);
	close(sockfd);
	return 1;
}
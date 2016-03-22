
void intiailiseGameObj(string msgRcvd)
{
	string splittedMessage[10];
	SplitString(msgRcvd, "/", splittedMessage);


	// Load Textures and initialise 
	for (int i = 0; i < 4; ++i)
	{
		string temp=string("SetMyGameObj")+string(";")+splittedMessage[i]+";";
		printf("Setting up .... %s\n",temp.c_str());
		processClientIPandPort(temp);
	}
	State=2;
}

void makeChangesToGameObj(string msgRcvd)
{
	
}
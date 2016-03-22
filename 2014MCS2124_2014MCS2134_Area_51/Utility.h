#include "header.h"


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

string convertIntToString(int i)
{
	char charString[155];
	snprintf(charString, 155, "%d", i);
	return string(charString);
}

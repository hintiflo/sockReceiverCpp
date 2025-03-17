/*
 * sockReceiver.cpp
 *
 *  Created on: 12.01.2025
 *      Author: Florian Hinterleitner
 */

#include <iostream>
#include <vector>
// #include <vector.h>
#include <cmath>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <type_traits>
#include <unistd.h>

#define INET_RX_BLOCKSIZE 4096
#define INFO "[INFO] "
#define ERRO "[ERRO] "
#define WARN "[WARN] "
#define DATA "[DATA] "
#define TODO "[TODO] "
#define EERO_POST endl << "    exiting program with error -1, goodbye!" << endl

using namespace std;

int main(int argc, char ** argv)
{
	cout << "***************************************************" << endl;
	cout << "    tcpReceiver, called with " << argc-1 << " args " << endl;
	cout << "***************************************************" << endl;


	int port = -1;
	string localIP = "-1.-1.-1.-1";
	// string localIP = "127.0.0.1";
	// 0. Sanitize input values or set std values
	if (argc < 2)
	{	localIP = "127.0.0.1";
		port = 10000;
		cout << INFO << "no params given, IP: " << localIP << ", port: " << port << endl;
	} 

	if (argc == 2)
	{	localIP = "127.0.0.1";
		auto argv1 = argv[1];
		port = atoi( argv1 );
		cout << INFO << "port param given, IP: " << localIP << ", port: " << port << endl;
	}

	if (argc == 3)
	{	auto argv1 = argv[1];
		auto argv2 = argv[2];
		localIP = argv1;
		
		if("localhost" == localIP)
			localIP  = "127.0.0.1";
		
		port = atoi( argv2 );
		cout << INFO << "port param given, IP: " << localIP << ", port: " << port << endl;
	}

	while(true)	// outer waiting loop
	{
		// 1.) create a listener socket
		int listener = socket(AF_INET, SOCK_STREAM, 0);
		if(0 > listener)
		{	cout << ERRO << "socket could not be created" << endl << "    exiting program with error -1, goodbye!" << endl;
			return -1;
		}
		int flag =1;
		if(	0 > setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) ||
			0 > setsockopt(listener, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(int))	)
		{	cout << ERRO << "socket settings to make address and port reuseable failed." << endl << "    proceeding anyway." << endl;
			// return -1;
		}
		cout << INFO << "socket created with FID: " << listener << endl;
		
		sockaddr_in listenerAddress;
		listenerAddress.sin_family = AF_INET;
		listenerAddress.sin_port = port;

		localIP = "127.0.0.1";
		// localIP = "192.168.1.1";
		cout << TODO << " localIP restricted to locahorst\n";
		cout << TODO << " Posrt readin incorrect\n";
		inet_pton(AF_INET, localIP.c_str(), &listenerAddress.sin_addr);
		
		// 2.) bind socket to adress/port
		char addrBuf[INET_ADDRSTRLEN];
		string sockIP = inet_ntop(AF_INET, &listenerAddress.sin_addr, addrBuf, INET_ADDRSTRLEN);
		int sockPort = ntohs(listenerAddress.sin_port);

		if(	0 > bind(listener, (sockaddr*)&listenerAddress, sizeof(listenerAddress))	)
		{	cout << ERRO << "listener can't bind to " << sockIP << "|" << sockPort << endl  << EERO_POST;;
			return -1;
		}
		cout << INFO << "listener bound to " << sockIP << "|" << sockPort << endl;
	
		// 3.) waiting for inbound connection
		if(	0 > listen(listener, SOMAXCONN)	)
		{	cout << ERRO << "listener can't be established" << endl <<  EERO_POST;;
			return -1;
		}
		cout << INFO << "listener listening on " << sockIP << "|" << sockPort << endl;

		// 4.) accepting incoming connection
		sockaddr_in clientAddr;
		int client;
		socklen_t clientAddrLen = sizeof(clientAddr);
		if( 0 >  (client = accept(listener, (sockaddr * ) &clientAddr, &clientAddrLen))	)
		{	cout << ERRO << "incoming connection could not be accpted" << EERO_POST;
			return -1;
		}
		cout << INFO << "incoming connection accepted on " << endl;;
		close(listener);
		cout << TODO << "why close listener?\n";
		
		cout << INFO << "connected to client: " << inet_ntop(AF_INET, &clientAddr.sin_addr, addrBuf, INET_ADDRSTRLEN) << "|" << ntohs( clientAddr.sin_port) << endl;

		char msgBuffer[INET_RX_BLOCKSIZE];
		// vector<char> msgBuffer(INET_RX_BLOCKSIZE);
		int rxCount = 0;
		while(true)	// inner connected loop
		{
			// 5.) receive incoming messages
			rxCount = recv(client, &msgBuffer, INET_RX_BLOCKSIZE, 0);
			// rxCount = recv(client, &msgBuffer[0], INET_RX_BLOCKSIZE, 0);
			if( 0 < rxCount )
			{	
				string msg = msgBuffer;
				// msg.append( msgBuffer.cbegin(), msgBuffer.cend() );
				// msgBuffer.clear();
				cout << INFO << "client " << msg;
				// cout << INFO << "client " << msgBuffer << endl;
				// 6.) echo message back to the client
				send(client, &(msgBuffer), rxCount, 0);
				memset(msgBuffer, 0, sizeof(msgBuffer));
			}else
			{	cout << ERRO << "RX error, client gone" << endl;
				break;
			}	
		}	// inner connected loop
		// 7.) close the connection gracefully
		close(client);
	}	// outer waiting loop


	return 0;
}

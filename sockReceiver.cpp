/*
 * sockReceiver.cpp
 *
 *  Created on: 12.01.2025
 *      Author: Florian Hinterleitner
 */




#include <iostream>
// #include <vector>
// #include <vector.h>
#include <cmath>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include <signal.h>

#include <stdlib.h>

// #include <algorithm>
#include <string.h>
// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <type_traits>
#include <unistd.h>

using namespace std;
#define INET_RX_BLOCKSIZE 4096

class cppSocket
{
	public:
		string localIP = "-1.-1.-1.-1";
		int listenerPort = -1;
		
		// string localIP = "127.0.0.1";
		cppSocket()
		{
			// cout << "i are cppSocket\n" ;
		}
		
		bool create()	// 1.)
		{	listenerFID = socket(AF_INET, SOCK_STREAM, 0);
			// bool options;
			int flag =1;
			if(	0 > setsockopt(listenerFID, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) ||
				0 > setsockopt(listenerFID, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(int))	)
				;
				// options = false;
			// else 
				// options = true;

			if(0 < listenerFID)
				return true;
			else 
				return false;
		}

		bool binding()	// 2.)
		{
			sockaddr_in listenerAddress;
			listenerAddress.sin_family = AF_INET;
			listenerAddress.sin_port = listenerPort;

			localIP = "127.0.0.1";
			// sock.localIP = "192.168.1.1";
			// cout << TODO << " localIP restricted to locahorst\n";
			// cout << TODO << " Posrt readin incorrect\n";
			inet_pton(AF_INET, localIP.c_str(), &listenerAddress.sin_addr);

			sockIP = inet_ntop(AF_INET, &listenerAddress.sin_addr, addrBuf, INET_ADDRSTRLEN);
			sockPort = ntohs(listenerAddress.sin_port);

			if(	0 > bind(listenerFID, (sockaddr*)&listenerAddress, sizeof(listenerAddress))	)
			{	// cout << ERRO << "listener can't bind to " << sockIP << "|" << sockPort << endl  << EERO_POST;;
				return false;
			}
			return true;
		}

		bool listening()	// 3.)
		{

			if(	0 > listen(listenerFID, SOMAXCONN)	)
				return false;
			return true;
		}

		bool accepting()
		{
			socklen_t clientAddrLen = sizeof(clientAddr);
			if( 0 >  (clientFID = accept(listenerFID, (sockaddr * ) &clientAddr, &clientAddrLen))	)
					return true;

			closeListener();
			clientPort = ntohs( clientAddr.sin_port);
			clientIP = inet_ntop(AF_INET, &(clientAddr.sin_addr), addrBuf, INET_ADDRSTRLEN);

			return true;
		}

		bool receiving(string * rxMsg)
		{
			char msgBuffer[INET_RX_BLOCKSIZE];
			int rxCount = recv(clientFID, &msgBuffer, INET_RX_BLOCKSIZE, 0);		
			if( 0 < rxCount )
				{	*rxMsg = msgBuffer;
					*rxMsg = (*rxMsg).erase((*rxMsg).find("\n") ,(*rxMsg).find("\n"));
					// clean the RX buffer
					memset(msgBuffer, 0, sizeof(msgBuffer));					
					return true;
				}
			else 
				return false;
		}

		bool sending(string txMsg)
		{	char * msgBuff = (txMsg.append("\n")).data();
			send(clientFID, msgBuff, strlen(msgBuff), 0);
			return true;
		}
		
		
		void closeListener()		{	close(listenerFID);		}
		void closeClient()			{	close(clientFID);		}	
			
		string getSockIP()			{	return sockIP;			}
		int  getSockPort()		{	return sockPort;			}

		string getClientIP()			{	return clientIP;			}
		int  getClientPort()		{	return clientPort;			}
	
	private:
		int clientFID = -1;
		string rxMsg = "";
		int listenerFID = -1;
		string sockIP = "";
		int sockPort = -1;
		char addrBuf[INET_ADDRSTRLEN];
		sockaddr_in clientAddr;
		int clientPort = -1;
		string clientIP = "";
	
	
};

class KeyExcept : public std::exception
{
	public:
		KeyExcept(int signal) : Signal(signal){}
	int Signal;
};


void signalException(int signal)
{	
	throw KeyExcept(signal);
}

#define INFO "[INFO] "
#define ERRO "[ERRO] "
#define WARN "[WARN] "
#define DATA "[DATA] "
#define TODO "[TODO] "
#define EERO_POST endl << "    exiting program with error -1, goodbye!" << endl

string clientGoneMsg = "client gone from port ";
string clientRxMsg = "client ";
string clientPortMsg = "client address: ";
string keyExceptMsg = "Ctrl+C irq";
string serverStopMsg = "server out";


int main(int argc, char ** argv)
{
	cppSocket sock;

	struct sigaction signalHandler;
	signalHandler.sa_handler = signalException;
	sigemptyset(&signalHandler.sa_mask);
	signalHandler.sa_flags = 0;
	sigaction(SIGINT, &signalHandler, NULL);


	cout << "***************************************************" << endl;
	cout << "    tcpReceiver, called with " << argc-1 << " args " << endl;
	cout << "***************************************************" << endl;

	// 0. Sanitize input values or set std values
	if (argc < 2)
	{	sock.localIP = "127.0.0.1";
		sock.listenerPort = 10000;
		cout << INFO << "no params given, IP: " << sock.localIP << ", port: " << sock.listenerPort << endl;
	} 

	if (argc == 2)
	{	sock.localIP = "127.0.0.1";
		auto argv1 = argv[1];
		sock.listenerPort = atoi( argv1 );
		cout << INFO << "port param given, IP: " << sock.localIP << ", sock.listenerPort: " << sock.listenerPort << endl;
	}

	if (argc == 3)
	{	auto argv1 = argv[1];
		auto argv2 = argv[2];
		sock.localIP = argv1;

		if("localhost" == sock.localIP)
			sock.localIP  = "127.0.0.1";

		sock.listenerPort = atoi( argv2 );
		cout << INFO << "port param given, IP: " << sock.localIP << ", port: " << sock.listenerPort << endl;
	}

	try
	{	while(true)	// outer waiting loop
		{
			// 1.) create a listener socket
			if( !sock.create() )
			{	cout << ERRO << "socket could not be created" << endl << "    exiting program with error -1, goodbye!" << endl;
				return -1;
			}

			// 2.) bind socket to adress/port
			if( !sock.binding() )
			{	cout << ERRO << "couldnt bind listener to " << sock.getSockIP() << "|" << sock.getSockPort() << endl  << EERO_POST;
				return -1;
			}	

			// 3.) waiting for inbound connection
			if( !sock.listening() )
			{	cout << ERRO << "listener can't be established" << endl <<  EERO_POST;;
				return -1;
			}
			cout << INFO << "listener listening on " << sock.getSockIP() << "|" << sock.getSockPort() << endl;

			if( !sock.accepting() )
			{	cout << ERRO << "incoming connection could not be accpted" << EERO_POST;
				return -1;
			}
			cout << INFO << clientPortMsg << sock.getClientIP() << "|" << sock.getClientPort() << endl;

			while(true)	// inner connected loop
			{	// 5.) receive incoming messages
				string rxMsg = "";
				if( sock.receiving( &rxMsg ) )
				{	cout << INFO << clientRxMsg << sock.getClientPort() << ": " << rxMsg << endl;
					// 6.) echo message back to the client
					sock.sending(rxMsg);
				}else
				{	cout << INFO << clientGoneMsg << sock.getClientPort() << endl;
					break;
				}
			}	// inner connected loop
			// 7.) close the connection 
			sock.closeClient();
		}	// outer waiting loop
	}	// try block

	catch(KeyExcept& except)
	{	sock.closeListener();
		cout << WARN << keyExceptMsg << except.Signal << endl;
	}

	cout << INFO << serverStopMsg << endl;
	return 0;
}
